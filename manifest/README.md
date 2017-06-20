#  Service Manifest Runtime Builder

## Table of Contents

1. [INTRODUCTION](#introduction)

2. [BUILD STEP](#build-step)

  2.1 [Part 1 First Time Registration](#part-1-first-time-registration)

  + 2.1.1 [Preconditions](#preconditions)
  
  + 2.1.2 [Add the manifest-runtime-production repository as a subdirectory to your component](#add-the-manifest-runtime-production-repository-as-a-subdirectory-to-your-component)

  + 2.1.3 [Customize your service manifest file](#customize-your-service-manifest-file)
  
  + + 2.1.3.1 [Rules and Requirements](#rules-and-requirements)

  + + 2.1.3.2 [Description of Fields](#description-of-fields)

  + + 2.1.3.3 [Magic Words](#magic-words)

  + 2.1.4 [Validate your Service Manifest File](#validate-your-service-manifest-file)

  + 2.1.5 [Add and Commit the Service Manifest](#add-and-commit-the-service-manifest)
  
  + 2.1.6 [Build the Runtime Library](#build-the-runtime-library)
  
  + 2.1.7 [Create an Installable Package of the Library and Register your Service Manifest](#create-an-installable-package-of-the-library-and-register-your-service-manifest)
  
  2.2 [Part 2 Subsequent Build Steps](#part-2-subsequent-build-steps)

  2.3 [Part 3 Update of the Service Metadata](#part-3-update-of-the-service-metadata)
  
  2.4 [Build Integration Examples](#build-integration-examples)
    
3. [DEPLOY STEP](#deploy-step)

  3.1 [Deployment of the shared library on VSI or Bare Metal servers](#deployment-of-the-shared-library-on-vsi-or-bare-metal-servers)

  + 3.1.1 [Deployment of the shared library into a VSI or Bare Metal server running Ubuntu](#deployment-of-the-shared-library-into-a-vsi-or-bare-metal-server-running-ubuntu)

  + 3.1.2 [Deployment of the shared library into a VSI or Bare Metal server running CentOS/RedHat Enterprise Linux](#deployment-of-the-shared-library-into-a-vsi-or-bare-metal-server-running-centos/redhat-enterprise-linux)

  3.2 [Deployment of the Shared Libary into Container Images](#deployment-of-the-shared-library-into-container-images)

  3.3 [Deploy the Cloud Foundry Based Package](#deploy-the-cloud-foundry-based-package)

4. [HOW TO CONTRIBUTE](#how-to-contribute)

5. [TROUBLESHOOTING](#troubleshooting)

  5.1 [Git Repository Related Issues](#git-repository-related-issues)

  5.2 [Connection Problems](#connection-problems)

  5.3 [All Other Issues](#all-other-issues)




## INTRODUCTION

This document is referenced by CloudServiceContract v2.0 chapter 3.2. 

It covers the BUILD STEP and the DEPLOY STEP which are described in the chapters below.

Please note: if something goes wrong please check the chapter [Troubleshooting](#troubleshooting) at the end of this document.

## BUILD STEP

The following chapters describe the steps necessary when you build your service offering.
The build step is identical for all different types of services/applications independent whether your service 

- is running on Softlayer's VSI or Bare Metall servers
- is embedded in an container
- is running as Bluemix's CloudFoundry application

Part 1 lists the steps you will take when registering your service for the first time.
 
Part 2 shows the steps in subsequent builds. 

Part 3 depicts the steps required when you update your service's metadata.

### Part 1 First Time Registration

Supported operating systems are:

- Ubuntu 14
- Ubuntu 16
- CentOS 7 / Redhat Enterprise Linux 7
- CentOS 6 / Redhat Enterprise Linux 6 

This procedure can not be executed on MacOS or Windows based workstations. 

Please consider: 

- Microservices/Components running on VSI/Bare Metal: 
  If you run a debian-alike OS then select to use Ubuntu for your build step. 
  If you run a RedHat-alike OS then select to use CentOS or RedHat Enterprise Linux for your build step. 

- Microservices/Components running in a Container: 
  If you run a debian-alike Container-OS then select to use Ubuntu for your build step.
  If you run a RedHat-alike Container-OS then select to use use CentOS or RedHat Enterprise Linux for your build step. 

- Microservices/Components running on Cloud Foundry:
  Select to use Ubuntu for you build step


#### Preconditions

You must have the following:

- A user account on IBM Github Enterprise.
- A Linux based build environment with the minimal install of gnu make and gnu compiler collection (the basic install of every modern linux).
- The Linux OS release/version of your build environment must correspond to your deployment target OS.
- The git packages (git version must be 1.7.11 or later) installed on your Linux build environment.
- If your git tooling is of an earlier version (as this is the case e.g. on RedHat 6) then upgrade to 1.7.11 or later (as descibed on this page: http://tecadmin.net/install-git-2-0-on-centos-rhel-fedora/).
- Your git tool needs the "git subtree" command extension to be installed. If an invocation of "git subtree" indicates that there is no "subtree" command, then add it to your git installation as described on this page: https://engineeredweb.com/blog/how-to-install-git-subtree/ (make sure the git-subtree module is installed in the path as returned by the 'git --exec-path' command).
- Make sure the gcc-c++ package is installed on your build server (ask your package manager if this is the case). 
  - Example for RedHat 6 / CentOS 6:
     - Execute: yum groupinstall "Development tools" 
     - if "rpm -qa | grep gcc-c++" and "whereis cc1" doesn't return a meaningful value, then install it using "yum install gcc-c++". 
  - Example for Ubuntu: 
     - Execute: "apt-get install build-essential"
- Running the build step on Ubuntu requires lintian and fakeroot to be installed. Please run "sudo apt-get install lintian" and "sudo apt-get install fakeroot". 

#### Add the manifest-runtime-production repository as a subdirectory to your component
 
1. In the root of your component repository, include the manifest-runtime-production project (this will create a 'manifest' subdirectory):

  `git remote add manifest-runtime-production git@github.ibm.com:CloudTools/manifest-runtime-production.git`
   
  and then:

  `git subtree add --prefix=manifest manifest-runtime-production master --squash`

  Note: refer to section [Troubleshooting](#troubleshooting) if you run into issues in this step.

2. Generate your initial servicemanifest.json file:

  This step is needed only once.
  
  ```
  Change into directory "manifest", then execute:
      make generate   
  ```

  It will create a template servicemanifest.json file under subdirectory "manifest/res/". Verify that the file exists.

#### Customize your service manifest file

This needs to be done only for initial creation of the servicemanifest.json or if updates are needed.
  
The initial file template looks like this:

```javascript
{
    "manifest_version": "1.0.0.0",
    "service_name": "<<Your Catalog Service Name>>",
    "component_name": "<<Your Component Name>>",

    "cname": "<<one of: bluemix/internal/staging/{customerId for dedicated/local}>>",
    "ctype": "<<one of: public/dedicated/local>>",
    "component_instance": "<<Your component instance identifier>>",
    "scope": "<<Your component instance scope identifier>>",
    "region": "<<Your component instance region identifier>>",
    "resource_type" : "<<Your Resource Type identifier>>",
    "resource" : "<<Your Resource Id identifier>>",
    
    "tenancy" : "<<Your Tenancy model - one of single/shared>>",  
    "pager_duty" : "<<Your PD Escalation Policy URL>>",
    "team_email" : "<<Your Team E-mail Alias>>",
    "bailey_url" : "<<Bailey URL>>",
    "bailey_project" : "<<Bailey Project Name>>",
    "repo_url" : "<<GHE Repo URL>>",
    
    "softlayer_account": "<<The account for SL resources provisioned to this component if applicable>>",
    "softlayer_account_api_key_id": "RESERVED FOR FUTURE USE",
    
   "bluemix_account": "<<The Bluemix Account for resources provisioned to this component if applicable>>",
    "bluemix_account_api_key_id": "RESERVED FOR FUTURE USE"

    "security_status_url": "<<URL for your Security Posture Status Page>>",
    "static_sec_analyze_url": "<<URL for the current results of your static analysis security scan>>",
    "customer_ticket_repo_url": "<<URL to your Customer Tickets Repository>>",
    "runbook_repo_url": "<<URL For your Runbook repository for this component>>",
    "continuity_plan_url" : "<<URL For your Business Continuity Plan for this component or the whole service>>"                
}
```

##### Rules and Requirements

The `servicemanifest.json` is a json formatted flat data structure containing key-value pairs. 

- **All** keys listed above **must** exist in the servicemanifest.json file with the following two exceptions: the keywords ```softlayer_account_api_key_id```, ```bluemix_account_api_key_id``` can be omitted.
- The following keys **require** a value: ```manifest_version```, ```service_name```, ```component_name```
- All other keys must exist, but the value is optional. If not set, the value must be an empty string (```""```)
 
##### Description of Fields

Notation:

- _required value_ vs. _optional value_
  - _required value_: This tag refers to keywords which require a value. For instance the keyword ```service_name``` requires a value. Example: ```"service_name" : "my-cloud-service"```
  - _optional value_: This tag refers to keywords which can have empty values. An empty value is represented by an empty string (note: The keywords with empty values must be included in the servicemanifest.json). For instance the keyword ```scope``` can have an empty value, e.g.: ```"scope" : ""```
- _static value_ vs. _instance specific value_ 
  - _static value_: This tag refers to values which are valid for the service definition and all of its running instances. Examples for keywords with static values are the ```service_name``` or the ```component_name```. (Note, for a key-value pair requiering a _static value_ the usage of "magic words" is not allowed). 
  - _instance specific value_: This tag refers to values which are determined at deployment (instance specific). An example for an instance specific value is ```region```. When you deploy multipe instances of the same component in different regions, then this value varies per instance. The preferred option for an _instance specific value_ is that you specify a "magic word". A "magic word" is a placeholder that is resolved by the shared libary at runtime. For example, one could specify `"region": "$env('REGION')"` in the servicemanifest.json. This would mean that after deployment the shared libary is able to retrieve the instance specific value for ```region``` from the environment variable ```$REGION``` on a deployed Softlayer VSI server. For further details please have a look at the _magic words_ section below. 

Now let's have a look at the servicemanifest.json _block by block_:

- The Header Block
  - ```manifest_version```: Do Not Touch. Currently the value needs to be "1.0.0.0" and is used by the Cloud Tools team to manage the version of the manifest format.
- The CRN Block (see the detailed [CRN specification](https://github.ibm.com/ibmcloud/builders-guide/blob/master/specifications/crn/CRN.md))
  - ```service_name``` ( _required value_ , _static value_ ): Allowed characters are alphanumeric, only lower case, no spaces or special characters other than "-". For Bluemix Services, this is the canonical service name as represented in the Bluemix Catalog. `cf marketplace` will return the list of services. For other services, you can decide on the appropriate name (i.e. IBM Container Service has chosen 'containers'). Once chosen, this should not change as it represents the public interface name for the service the component is part of. 
    - Example: ```"service_name" : "my-cloud-service"```
  - ```component_name``` ( _required value_, _static value_ ): The name of your service component (e.g. microservice). Allowed characters are alphanumeric, only lower case, no spaces or special characters other than "-". 
     - Example: ```"component_name" : "my-cloud-component"```
  - ```cname``` ( _optional value_ , _instance specific value_ ): One of the following: _bluemix_, _internal_, _staging_ or the Customer ID (for Dedicated/Local deployments). Allowed characters for the Customer ID are alphanumeric, only lower case, no spaces or special characters other than "-". 
    - Example: ```"cname" : "$env('CNAME')"```
  - ```ctype``` ( _optional value_ , _static value_ ): One of the following: _public_, _dedicated_ or _local_. 
    - Example: ```"ctype" : "public"```
  - ```component_instance``` ( _optional value_ , _instance specific value_ ): An identifier for the instance of the service. This needs to be an ASCII string without colons.
    - Example: ```"component_instance" : "$env('COMPONENT_INSTANCE')"```
  - ```scope``` ( _optional value_ , _instance specific value_ ): The CRN Scope value. This needs to be an ASCII string without colons. For details refer to the [CRN specification](https://github.ibm.com/ibmcloud/builders-guide/blob/master/specifications/crn/CRN.md).
    - Example: ```"scope" : "$env('SCOPE')"```
  - ```region``` ( _optional value_, _instance specific value_ : The region the component is deployed into. Format: three capital characters A...Z followed by two digits 0...9 or two lower case characters a...z followed by a hyphen and at least two lower case characters. 
    - Example: ```"region": "$env('REGION')"```
  - ```resource_type``` ( _optional value_ , _static value_ ): An identifier for the 'type' of this component. Allowed characters are alphanumeric, only lower case, no spaces or special characters other than "-".
    - Example: ```"resource_type" : ""```
  - ```resource``` ( _optional value_ , _instance specific value_ ): An identifier for specific resource type represented by this component. Useful for components that are sub parts of an overall service instance. Format: ASCII string without colons.
    - Example: ```"resource": "$env('RESOURCE')"```
- The Operations Support Block
  - ```tenancy``` ( _optional value_ , _static value_ ): One of two values: _single_ - means an instance is deployed per account/tenant. _shared_ means the componet is used by multiple tenants. This is from an architectural perspective, and does not change if the deployment is specifically single tenant (i.e. Local or Dedicated).
    - Example: ```"tenancy" : "single"```
  - ```pager_duty``` ( _optional value_ , _static value_ ): The URL of the PagerDuty Escalation Policy for the squad that builds and deploys the component.
    - Example: ```"pager_duty" : "https://bluemix.pagerduty.com/escalation_policies#ABCDEF"```
  - ```team_email``` ( _optional value_ , _static value_ ): The email address of the squad that builds and deploys the component.
    - Example: ```"team_email" : "oursquad@us.ibm.com"```
  - ```bailey_url``` ( _optional value_ , _static value_ ): The URL of the Bailey instance for the service.
    - Example: ```"bailey_url" : "https://www.bluemix.net"```
  - ```bailey_project``` ( _optional value_ , _static value_ ): The name of the Bailey project for the service. Can be any ASCII string.
    - Example: ```"bailey_project" : "bluemix-platform"```
  - ```repo_url``` ( _optional value_ , _static value_ ): The URL of the source code control repository for project for the component of the service.
    - Example: ```"repo_url" : "https://github.ibm.com/CloudTools/manifest-runtime-production.git"```
- The Authentication Block
  - ```softlayer_account``` ( _optional value_ , _instance specific value_ ): If your component deploys to SL VSIs or Bare Metals, this is the account under which those instances are provisioned. Format: ASCII string.
    - Example: ```"softlayer_account" : "1122334455"```
  - ```softlayer_account_api_key_id```: For future use - this value is ignored and can be omitted. Format: any ASCII string. 
    - Example: ```"softlayer_account_api_key_id" : "RESERVED FOR FUTURE USE"```
  - ```bluemix_account``` ( _optional value_ , _instance specific value_ ): If your component deploys to Bluemix facilities (Cloud Foundry, Containers, Whisk), this is the account under which those instances are provisioned. Format: any ASCII string.
    - Example: ```"bluemix_account" : ""```
  - ```bluemix_account_api_key_id```: For future use - this value is ignored and can be omitted. Format: any ASCII string.
    - Example: ```"bluemix_account_api_key_id" : "RESERVED FOR FUTURE USE"```
  
- The Various Block:
  - ```security_status_url``` ( _optional value_ , _static value_ ) : The URL for your Security Posture Status Page.
    - Example: ```"security_status_url" : "https://w3.ibm.com/oursecuritystatus"```
  - ```static_sec_analyze_url``` ( _optional value_ , _static value_ ): The URL for the current results of your static analysis security scan.
    - Example: ```"static_sec_analyze_plan_url" : "https://w3.ibm.com/ourstaticanalysissecurityscan"```
  - ```customer_ticket_repo_url``` ( _optional value_ , _static value_ ): "The URL to your Customer Tickets Repository.
    - Example: ```"customer_ticket_repo_url" : "https://jazzop27.rtp.raleigh.ibm.com:9443/ourproject#action=com.ibm.team.dashboard.viewDashboard"```
  - ```runbook_repo_url```: The URL For your Runbook repository for this component.
    - Example: ```"runbook_repo_url" : "https://github.ibm.com/CloudTools/runbooks.git"```
  - ```continuity_plan_url``` ( _optional value_ , _static value_ ): The URL for your Business Continuity Plan for this component or the whole service.  
    - Example: ```"continuity_plan_url" : "https://w3.ibm.com/ourcontinuityplan"```

##### Magic Words

Magic words are special metadata placeholders that can be used in place of an _instance specifc value_ in the servicemanifest.json. You might be surprised that the servicemanifest.json includes key-value pairs with instance (runtime) specific values. The prefered option for those key-value pairs is to use use magic words. This will allow that after deployment the shared library can automatically "retrieve" at runtime the instance specific value and provide it for further use. E.g. this allows that the shared library will be able to automatically generate Cloud Resource Names (CRNs) which of course contain instance specific (runtime) data. 

One Magic Word is implemented so far:

```
"$env('<environment variable name>')"
```

```$env``` is used as a value for instance specific properties and will result in the service manifest library substituting the named environment variable from the deployed runtime for the property. For example specifying `"instance": "$env('CF_INSTANCE_GUID')"`, and deploying the compiled service manifest into a Cloud Foundry deployed application will result in the Cloud Foundry ```CF_INSTANCE_GUID``` environment variable being used as the instance id in all service manifest library calls. This is a useful facility that will let you set values in deployment scripts, or rely on values generated by runtimes like Cloud Foundry, Containers, or even SoftLayer in your Service Manifest definition.

#### Validate your Service Manifest File

Once you have customized or updated your servicemanifest.json, or whenever you pull a new version of the `/manifest` subdirectory, you should validate it with:

  ```
  Change into directory "manifest", then execute:
  
    make validate
    
  ```
  
This will execute a tool that reads the servicemanifest.json using the same parsing library as the service manifest runtime and will identify any simple syntax issues or missing properties. 

#### Add and Commit the Service Manifest 

You customized the servicemanifest.json to your service and should check it into your own service's repository. The following is only an example - if you use a GIT repository for your service - and probably doesn't follow your checkin guidelines:

  ```
  Change into your component's root directory, then execute:
  
    git add manifest/res/servicemanifest.json
    git commit -m "Initial Service Manifest Json"
    git push
    
  ```

_NOTE:_ If you are using a different source code repository than Github Enterprise for your component, then this is a great opportunity to migrate. 

#### Build the Runtime Library

Build the manifest runtime library: 

  ```
  Change into directory "manifest", then execute:
  
    make
    
  ```

`make` will produce the _libibmmanifest.so.1.x.y_ in the manifest/wrapper folder. This is the runtime manifest shared library - customized to your service offering - that will be packaged into installables with the next step and deployed to your service's runtime in the deploy step.

#### Create an Installable Package of the Library and Register your Service Manifest

Now build the installable package using the following command. 

  ```
  Change into directory "manifest", then execute:
  
    make package
    
  ```
  
This will also register your service in the Service Registry.

### Part 2 Subsequent Build Steps

The registration of your service whenever you run your component's/service's build.

Adding the registration step to your service build pipeline ensures that the information provided about your offerings stay current. Upon each build execute the following steps:

  ```
  In the root of your component repository, include the manifest-runtime-production
  project and fetch the latest code from the manifest-runtime-production repository:
  
    git remote add manifest-runtime-production git@github.ibm.com:CloudTools/manifest-runtime-production.git
    (skip this step if you receive a message that the repository exists already)
  
    git subtree pull --prefix=manifest manifest-runtime-production master -m "Pull manifest-runtime-production updates" --squash

  Then run the servicemanifest validation, build the manifest library and run the
  registration step with the following commands:
  
    cd manifest
    make validate
    make
    make package
  
  ("make package" registers the servicemanifest with the Service Registry)
    
  ```

  Note: refer to section [Troubleshooting](#troubleshooting) if you run into issues in this step.
      
### Part 3 Update of the Service Metadata

Whenever the metadata describing your service offering change, execute the following steps:

  ```
  In the root of your component repository, include the manifest-runtime-production project and fetch the latest code from the manifest-runtime-production repository:
  
    git remote add manifest-runtime-production git@github.ibm.com:CloudTools/manifest-runtime-production.git
    (skip this step if you receive a message that the repository exists already)
  
    git subtree pull --prefix=manifest manifest-runtime-production master -m "Pull manifest-runtime-production updates" --squash

  Then modify the metadata for your service in manifest/res/servicemanifest.json.

  Then run the servicemanifest validation, build the manifest library and run the
  registration step with the following commands:
  
    cd manifest
    make validate
    make
    make package
  
  Then push the modified servicemanifest.json to your service's repository.
  Change into your component's root directory and execute:
    
    git add manifest/res/servicemanifest.json
    git commit -m "Updated Service Manifest Json"
    git push
    
  ```
  
  Note: refer to section [Troubleshooting](#troubleshooting) if you run into issues in this step. 

### Build Integration Examples

If you want to integrate the process shown above with your build automation, see examples below (please note, these are real-world examples, you need to customize them to your needs).

   [Travis yml file] (https://github.ibm.com/CloudTools/ServiceRegistry/blob/master/.travis.yml)
   
   (more to come)
    
## DEPLOY STEP

This chapter is work in progress.

The following sections describe the steps necessary when you deploy your service/application components.
The deploy step differs for services/applications, depending whether your components ...

- ... are running on Softlayer's VSI or Bare Metall servers
- ... are embedded in containers
- ... are running as Bluemix's CloudFoundry applications

In the following sections the alternative deployment approaches are explained. 

### Deployment of the shared library on VSI or Bare Metal servers

Here, you have to extend your automated deployment automation (e.g. using Jenkins) to add the installation of a package which embeds the shared libary along with your component installation. At the end of the installation of the shared libary package a script is automatically invoked that will emit a build metric to the metric service and write a log message to the logging service.

#### Deployment of the shared library into a VSI or Bare Metal server running Ubuntu 

Assume you executed "make package" on a build server running Ubuntu (Debian based), then this created the following build pack: ../manifest/dist/libibmmanifest1.deb.

Stage the file to your target machine and install it using the following commands (Note: this step has to be embedded into your automated deployment step, e.g. Jenkins):

```
Install the deb:
    dpkg -i libibmmanifest1.deb 

Validate if it has been installed:
    dpkg -s libibmmanifest1
    dpkg -l libibmmanifest1
    dpkg -L libibmmanifest1

The following library must be present as well:
    ls -latr /usr/lib/libibmmanifest*
    
Remove the asset:
    dpkg -r libibmmanifest1 

```

At the end of the package installation a metric and a log message is sent. 

#### Deployment of the shared library into a VSI or Bare Metal server running CentOS/RedHat Enterprise Linux 

Assume you executed "make package" on a build server running CentOS/RedHat Enterprise Linux, then this created the following build pack: ../manifest/dist/rpm/RPMS/x86_64/libibmmanifest-1.0-1.x86_64.rpm.

Stage the file to your target machine and install it using the following commands (Note: this step has to be embedded into your automated deployment step, e.g. Jenkins):


```
Install the rpm:
    rpm -i libibmmanifest-1.0-1.x86_64.rpm 

Validate if it has been installed:
    rpm -qa | grep -i libibmmanifest

The following library must be present as well:
    ls -latr /usr/lib/libibmmanifest*
```

At the end of the package installation a metric and a log message is sent. 

### Deployment of the Shared Libary into Container Images

Container images running cloud services or applications have to embed the Shared Library which was generated in the build step (A). The Shared Libary is provided in different packaging alternatives:

- either as rpm package (on CentOS/RedHat Enterprise Linux build server) or as deb package (on Ubuntu build server)
- as tarball

At the time when you download/build your container image, it is necessary that an automated step has to be added to install the shared libary executables into the container image. This can be done by either extracting the tarball into the container image or by installing the shared library using a package manager.

Important: Please consider that the container image build step is seen as deploy step of control point 1. Later deployments of container instances are out of scope for the control point 1.

### Deploy the Cloud Foundry Based Package

### Deployment of the Shared Libary into Cloud Foundry Applications

#### Deployment steps for Node.js based CF Applications

Assume you executed `make package` on your build server running Ubuntu, then this created the following tarball: `../manifest/dist/libibmmanifest.tar.gz`. For installing the Shared Library into your Cloud Foundry applications you have to enhance your project source code by executing the following steps:
- copy the tarball `../manifest/dist/libibmmanifest.tar.gz` from your build server into the project root directory of your application
- create or update a file named `.profile` in the project root directory of your CF application with the following code:

```
function cegtools_activate {
  if [ ! -f libibmmanifest.tar.gz ] ; then
    echo "ERROR: could not find tarball libibmmanifest.tar.gz in current directory $(pwd)."
    return 1
  fi
  mkdir -p cegtools
  tar -C cegtools -xvzf libibmmanifest.tar.gz
  # source the activation script, do not call it (in a sub-shell):
  . ./cegtools/usr/bin/cfactivate.sh
}

cegtools_activate
```

Now push your application (`CF push ...`) as usual in your deployment step. The file `.profile` will be executed immediately before each instance of your application starts. It will extract the tarball and invoke the activation script for the shared library.

#### Deployment steps for Cloud Foundry Applications using other Build Packs

This is under work.

## HOW TO CONTRIBUTE

If you want to contribute to the project "manifest-runtime" please use the GIT repository 
[manifest-runtime-release](https://github.ibm.com/CloudTools/manifest-runtime-release). This is the development repository ("non-production"/beta code version). Pull requests should be opened only on this ([manifest-runtime-release](https://github.ibm.com/CloudTools/manifest-runtime-release)) repository.

If you want to do further development testing on the built manifest library, you can choose to run `sudo make install` at this point. This will install the _libibmmanifest.so.1.x.y_ to the _/usr/lib_ folder and add it to the LD_LIBRARY_PATH so that it can be dynamically loaded by dependencies, including the tools in [this repository](https://github.ibm.com/CloudTools/manifest-runtime-clients).

## TROUBLESHOOTING

### Git Repository Related Issues

The process outlined above requires your service to use a GIT repository. The standard procedure is to add the manifest-runtime-production repository as a subtree to your service repository.

If you receive the error "Working tree has modifications. Cannot add." then either commit the local changes or revert back your local uncommitted changes (e.g. using "git checkout master"). Otherwise the "git subtree pull" command will refuse to update the local manifest subtree.

"git subtree" works only if your service's repository is a GIT repository. If you use a different version control system it's worth to consider switching to Github Enterprise. But as a workaround you might just checkout the "manifest-runtime-production" repo / 'master' branch using the following command: "git clone git@github.ibm.com:CloudTools/manifest-runtime-production.git manifest"

### Connection Problems

When using Travis based build the upload of the manifest to the ServiceRegistry server might go through a VPN tunnel to reach the blue network. The MTU of the tun0 interface (on the outer Travis VM) has an MTU of 1355. The containers which Docker creates have an eth0 with an MTU of 1500 so transmitted packets need to be fragmented inside the Travis VM before being sent over tun0. Under circumstances this might not work correctly and could cause the SSL handshake to fail when ceg_register tries to connect to the ServiceRegistry server.

One way round this is to run the containers with the --privilege flag so that the container code can run ifconfig to change the MTU on eth0 but it was a bit of a kludge.

You could create a new network which has an MTU of 1300. Any containers which are created using this network automatically inherit this MTU for their eth0 interface. It would have been simpler just to modify the existing Docker bridge network but unfortunately you cannot modify the default networks so I had to create a new one.

### All Other Issues

If you need help open an issue in the respective repository [service manifest production repository](https://github.ibm.com/CloudTools/manifest-runtime-production) or post a message in the ceg-service-contract Slack channel.

