def docker_images = ["pnd-rtklinux-docker-dev.vegistry.vg.vector.int/pnd-rtklinux-build-centos7:1.4",
                     "pnd-rtklinux-docker-dev.vegistry.vg.vector.int/pnd-rtklinux-build-ubuntu1804:1.1"]

//TODO
//def windows_profiles = 

//TODO replace with productive system
artifactoryServer = Artifactory.server 'vistrpndart1-test'
artifactoryBuildInfo = Artifactory.newBuildInfo()

def isTag(svnUrl) {
  def result = svnUrl.contains('tags')
  echo "-----> isTag() == ${result}"
  return result
}
def getTag(svnUrl) {
  def result = svnUrl.tokenize('/').last()
  echo "-----> getTag() == ${result}"
  return result
}

def buildAndTest(img, configType) {
    stage("Build ${configType}") {
        def buildDir = "build_${configType}"
        sh "mkdir ${buildDir}"
        sh "chmod +x build.sh"
        dir(buildDir) {
          def build_cmd = "../build.sh ${configType} ${env.tagNr}"
          if (img.contains('centos')) {
            sh "scl enable llvm-toolset-6.0 '${build_cmd}'"
          } else {
            sh "${build_cmd}"
          }
        }
    }
    stage("Tests ${configType}") {
      echo "-----> TODO ctest"
        //runCtest(buildDir, configType, "UnitTest")
    }
}

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
              echo '-----> checkout results:\n' + checkoutResults.toString()
              env.isTag = isTag(checkoutResults.SVN_URL)
              if (env.isTag) {
                env.tagNr = getTag(checkoutResults.SVN_URL)
              } else {
                env.tagNr = ""
              }
              echo '-----> env:\n' + sh(script: 'env|sort', returnStdout: true)

            }
            docker.withRegistry('https://pnd-rtklinux-docker-dev.vegistry.vg.vector.int/', 'fa92756a-62a2-4436-9a66-ceb0c2c109a2') {
              docker.image(img).inside {

                buildAndTest(img, "debug")

                buildAndTest(img, "release")

                if (env.isTag) {
                  stage('Deploy') {
                    artifactoryServer.upload buildInfo: artifactoryBuildInfo, spec: """{
                          "files": [
                              {
                              "pattern": "build/*.tgz",
                              "target": "pnd-rtklinux-generic-dev-local/OpenRTI/upload/${env.tagNr}/",
                              "flat" : "true"
                              }
                          ]
                      }"""
                  }
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