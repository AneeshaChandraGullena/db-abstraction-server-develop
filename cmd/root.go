// © Copyright 2016 IBM Corp. Licensed Materials – Property of IBM.

package cmd

import (
	"fmt"
	"net"
	"os"
	"os/signal"
	"runtime"
	"syscall"

	"github.com/go-kit/kit/log"
	"github.com/spf13/cobra"

	"github.ibm.com/Alchemy-Key-Protect/db-abstraction-server/utils/logging"
	"github.ibm.com/Alchemy-Key-Protect/go-db-service/services/metadata"
	metaSpec "github.ibm.com/Alchemy-Key-Protect/go-db-service/services/metadata/protobuf"
	metaServices "github.ibm.com/Alchemy-Key-Protect/go-db-service/services/metadata/service"
	"github.ibm.com/Alchemy-Key-Protect/go-db-service/services/provision"
	provSpec "github.ibm.com/Alchemy-Key-Protect/go-db-service/services/provision/protobuf"
	provServices "github.ibm.com/Alchemy-Key-Protect/go-db-service/services/provision/service"
	"github.ibm.com/Alchemy-Key-Protect/kp-go-config"
	"github.ibm.com/Alchemy-Key-Protect/kp-go-consts"

	"golang.org/x/net/context"
	"google.golang.org/grpc"
)

var (
	config configuration.Configuration

	// mainSemver is set by build to denote the semver numbering
	mainSemver string

	// mainCommit is set by the build to denote the commit SHA1 of the build
	mainCommit string

	deployed = (runtime.GOOS == constants.LinuxRuntime)
)

func init() {
	// Set config
	config = configuration.Get()
}

func isDeployed() bool {
	return deployed
}

func validateVersion(config configuration.Configuration) {
	// ensure that the configuration file and binary file were built together
	configVersion := config.GetString("version.semver")
	if isDeployed() && mainSemver != configVersion {
		panic(fmt.Sprintf("Version mismatch enabled on %s: expected %s have %s ", runtime.GOOS, configVersion, mainSemver))
	}

	configCommit := config.GetString("version.commit")
	if isDeployed() && mainCommit != mainCommit {
		panic(fmt.Sprintf("Commit mismatch enabled on %s: expected %s have %s ", runtime.GOOS, configCommit, mainCommit))
	}
}

// rootCmd represents the base command when called without any subcommands
var rootCmd = &cobra.Command{
	Use:   "db-abstraction-server",
	Short: "IBM Key Protect DB service",
	Long:  `IBM Key Protect DB service provides access to all the microservices`,
	// Uncomment the following line if your bare application
	// has an action associated with it:
	Run: func(cmd *cobra.Command, args []string) {
		logger := logging.GlobalLogger()

		validateVersion(config)

		mainLogger := log.NewContext(logger).With("component", "main")
		mainLogger.Log("semver", mainSemver, "commit", mainCommit)

		errc := make(chan error, 2)
		ctx := context.Background()

		server := grpc.NewServer()

		grpcLogger := log.NewContext(logger).With("transport", "gRPC")

		// Metadata Service
		{
			service := metaServices.NewBasicService()
			service = metaServices.NewLoggingService(log.NewContext(logger).With("component", "metadata", "caller", log.DefaultCaller), service)

			databaseServiceMeta := metadataDB.MakeServer(ctx, service, grpcLogger)

			metaSpec.RegisterMetadataServer(server, databaseServiceMeta)
		}

		// Provision Service
		{
			service := provServices.NewBasicService()
			service = provServices.NewLoggingService(log.NewContext(logger).With("component", "provision", "caller", log.DefaultCaller), service)

			databaseServiceProv := provisionDB.MakeServer(ctx, service, grpcLogger)

			provSpec.RegisterProvisionServer(server, databaseServiceProv)
		}

		// listener can be used by multiple go routines at once
		ln, err := net.Listen("tcp", ":"+config.GetString("host.port"))
		if err != nil {
			errc <- err
			return
		}

		go func() {
			if errLog := grpcLogger.Log("address", ":"+config.GetString("host.port"), "msg", "listening"); errLog != nil {
				panic("Unable to Log HTTP transport")
			}
			errc <- server.Serve(ln)
		}()

		go func() {
			signalChan := make(chan os.Signal)
			signal.Notify(signalChan, syscall.SIGINT)
			errc <- fmt.Errorf("%s", <-signalChan)
		}()

		errSigLog := logger.Log("terminated", <-errc)
		if errSigLog != nil {
			panic("cannot log basic server info")
		}
	},
}

// SetVersion needs to be called by main.main() to set build version, so that the version commmand returns the value matching the build
func SetVersion(version string, commit string) {
	if version == "" {
		mainSemver = "0.0.0"
	} else {
		mainSemver = version
	}
	if commit == "" {
		mainCommit = "0000"
	} else {
		mainCommit = commit
	}
}

// Execute adds all child commands to the root command sets flags appropriately.
// This is called by main.main(). It only needs to happen once to the rootCmd.
func Execute() {
	if err := rootCmd.Execute(); err != nil {
		fmt.Println(err)
		os.Exit(-1)
	}
}
