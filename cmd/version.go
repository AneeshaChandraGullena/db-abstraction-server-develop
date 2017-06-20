// © Copyright 2016 IBM Corp. Licensed Materials – Property of IBM.

package cmd

import (
	"fmt"

	"github.com/spf13/cobra"
)

// versionCmd represents the version command
var versionCmd = &cobra.Command{
	Use:   "version",
	Short: "build version",
	Long: `The build process injects a semver build version into the binary, which can be cross verified to the configuration JSON files.
The binary and configuratin file semver value must match.`,
	Run: func(cmd *cobra.Command, args []string) {
		if ShowCommit != true {
			fmt.Println(mainSemver)
		} else {
			fmt.Println(mainCommit)
		}
	},
}

// ShowCommit tells the version command to show commit SHA1 instead of semver
var ShowCommit bool

func init() {
	rootCmd.AddCommand(versionCmd)

	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// versionCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	versionCmd.Flags().BoolVarP(&ShowCommit, "commit", "c", false, "Display commit id instead of semver")
}
