# Build and Create Release Workflow 
 
This GitHub Actions workflow automates the build and release process for your project. 
It compiles the project, packages the build artifacts, and creates a GitHub release with those artifacts. 
 
## Workflow Overview 
 
The workflow is triggered on: 
- Push events to tags matching the pattern v*.* 
- Manual dispatch using the workflow_dispatch event 
 
## Jobs 
 
### 1. Generate Build 
 
This job handles the build process. 
 
#### Steps: 
1. Checkout Repository: Clones the repository, including submodules. 
2. Install ARM gcc Toolchain: Installs the ARM GCC toolchain. 
3. Verify ARM gcc Version: Verifies the ARM GCC version. 
4. Make Release: Runs make release to generate the build. 
5. Upload Build Artifact: Uploads the build artifacts from the ./release directory. 
 
### 2. Create Release 
 
This job creates a GitHub release and uploads the build artifacts. 
 
#### Steps: 
1. Checkout Repository: Clones the repository. 
2. Get Tag Name: Retrieves the tag name from the environment variable. 
3. Download Build Artifact: Downloads the build artifacts uploaded in the generate job. 
4. Create GitHub Release: Creates a GitHub release and uploads the files from the ./release directory. 
 
## Usage 
 
To use this workflow, ensure your repository has a Makefile with a release target, and the necessary ARM GCC toolchain is compatible with your project setup. 
Push a tag matching v*.* to trigger the workflow, or manually trigger it through the GitHub Actions interface. 
The following commands will trigger the action: 

```
git add . 
git commit -m "v1.0"   
git tag -a v1.0 -m "Version 1.0"            
git push origin master --tags 
``` 

Please notice that tags are unique, so change them every time.
