def docker_images = ["pnd-rtklinux-docker-dev.vegistry.vg.vector.int/pnd-rtklinux-build-centos7:1.4",
                     "pnd-rtklinux-docker-dev.vegistry.vg.vector.int/pnd-rtklinux-build-ubuntu1804:1.1"]

def windows_profiles = 

//TODO replace with productive system
artifactoryServer = Artifactory.server 'vistrpndart1-test'
artifactoryBuildInfo = Artifactory.newBuildInfo()


def get_linux_stages(img) {
  return {
    stage(img) {
      node('docker && linux') {
        withEnv([
          "CC=clang",
          "CXX=clang++"
          ]) {
          try 
          {
            stage("Checkout") {
              def checkoutResults = checkout scm
              echo '-----> checkout results' + checkoutResults.toString()
              echo '-----> env' + sh(script: 'env|sort', returnStdout: true)
            }
            docker.withRegistry('https://pnd-rtklinux-docker-dev.vegistry.vg.vector.int/', 'fa92756a-62a2-4436-9a66-ceb0c2c109a2') {
              docker.image(img).inside {
                stage('Build') {
                  sh "mkdir build"
                  sh "chmod +x build.sh"
                  dir('build') {
                    //TODO build debug
                    //TODO dynamically determine version
                    def build_cmd = '../build.sh Release 1234'
                    if (img.contains('centos')) {
                      sh "scl enable llvm-toolset-6.0 '${build_cmd}'"
                    } else {
                      sh "${build_cmd}"
                    }
                  }
                }
                stage('Deploy') {
                  //TODO only deploy on tags
                  //TODO dynamically determine version
                  artifactoryServer.upload buildInfo: artifactoryBuildInfo, spec: """{
                        "files": [
                            {
                            "pattern": "build/*.tgz",
                            "target": "pnd-rtklinux-generic-dev-local/OpenRTI/upload/1234/",
                            "flat" : "true"
                            }
                        ]
                    }"""
                }
              }
            }
          }
          finally {
            cleanWs()
          }
        }
      }
    }
  }
}

pipeline {
  agent none
  options {
    buildDiscarder(logRotator(numToKeepStr: '10', artifactNumToKeepStr: '10'))
    timeout(time: 1, unit: "HOURS")
    skipDefaultCheckout(true)
  }
  stages {
    stage('Build') {
      steps {
        script {
          // all_stages = windows_profiles.collectEntries { pr, labels -> ["${pr}": get_windows_stages(pr, labels)]}
          all_stages = docker_images.collectEntries { img -> ["${img}": get_linux_stages(img)]}
          parallel all_stages
        }
      }
    }
  }
  // post {
  //   failure {
  //     emailext ( subject: '[Jenkins OpenRTI] OpenRTI Build Failed',
  //                mimeType: 'text/html',
  //                to: 'abc@vector.com',
  //                recipientProviders: [[$class: 'CulpritsRecipientProvider']],
  //                body: "The OpenRTI Build failed:  ${env.RUN_DISPLAY_URL} <br/> Please check if you are responsible for the build break.")
  //   }
  //   success {
  //     emailext ( subject: '[Jenkins OpenRTI] OpenRTI Build Succeeded',
  //                 mimeType: 'text/html',
  //                 to: 'abc@vector.com',
  //                 recipientProviders: [[$class: 'CulpritsRecipientProvider']],
  //                 body: "The OpenRTI Build succeeded:  ${env.RUN_DISPLAY_URL}")
  //   }
  // }
}