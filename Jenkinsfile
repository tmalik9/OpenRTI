//TODO replace with productive system
artifactoryServer = Artifactory.server 'vistrpndart1-test'
artifactoryBuildInfo = Artifactory.newBuildInfo()



def isTag(svnUrl) {
  def result = svnUrl.contains('tags')
  //echo "-----> isTag() == ${result}"
  return result
}
def getTag(svnUrl) {
  def result = svnUrl.tokenize('/').last()
  //echo "-----> getTag() == ${result}"
  return result
}

def checkout() {
  stage("Checkout") {
    def checkoutResults = checkout scm
    //echo '-----> checkout results:\n' + checkoutResults.toString()
    env.isTag = isTag(checkoutResults.SVN_URL)
    if (env.isTag) {
      env.tagNr = getTag(checkoutResults.SVN_URL)
    } else {
      env.tagNr = ""
    }
    //echo '-----> env:\n' + sh(script: 'env|sort', returnStdout: true)
  }
}

def buildAndTest_linux(img, configType) {
    def buildDir = "build_${configType}"
    stage("Build ${configType}") {
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
    stage("Test ${configType}") {
        dir(buildDir) {
            sh "ctest --verbose --build-config ${configType}"
        }
    }
}

def stages_linux(build_name, comp_env, img) {
  return {
    stage(build_name) {
      node('docker && linux') {
        withEnv(comp_env) {
          try 
          {
            checkout()

            docker.withRegistry('https://pnd-rtklinux-docker-dev.vegistry.vg.vector.int/', 'fa92756a-62a2-4436-9a66-ceb0c2c109a2') {
              docker.image(img).inside {

                buildAndTest_linux(img, "debug")

                buildAndTest_linux(img, "release")

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


def buildAndTest_win(configType, additionalCmakeArgs) {
    def buildDir = "build_${configType}"
    cmakeBuild(
        installation: 'InSearchPath', 
        buildDir: "${buildDir}",
        buildType: "${configType}",
        generator: "Visual Studio 16 2019",
        cleanBuild: true,
        cmakeArgs: "${additionalCmakeArgs} -DCMAKE_BUILD_TYPE=${configType}",
        steps: [[
            args: "--config ${configType}",
            withCmake: true // "--build ." Argument
        ]]
    )
    stage("Test ${configType}") {
      ctest(
          installation: 'InSearchPath',
          arguments: "--verbose --build-config ${configType}"
      )
    }
}

def stages_win(build_name, additionalCmakeArgs) {
  return {
    stage(build_name) {
      node('buildtools2019') {
        withEnv([]) {
          try 
          {
            checkout()

            buildAndTest_win("debug", additionalCmakeArgs)

            buildAndTest_win("release", additionalCmakeArgs)

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
          def builds = [:]

          // Linux flavours
          env_clang = ["CC=clang","CXX=clang++"]
          env_gcc = ["CC=gcc","CXX=g++"]
          docker_image_centos = "pnd-rtklinux-docker-dev.vegistry.vg.vector.int/pnd-rtklinux-build-centos7:1.4"
          docker_image_ubuntu = "pnd-rtklinux-docker-dev.vegistry.vg.vector.int/pnd-rtklinux-build-ubuntu1804:1.1"

          builds.put("centos7 clang", stages_linux("centos7 clang", env_clang, docker_image_centos))
          builds.put("ubuntu1804 clang", stages_linux("ubuntu1804 clang", env_clang, docker_image_ubuntu))
          builds.put("ubuntu1804 gcc", stages_linux("ubuntu1804 gcc", env_gcc, docker_image_ubuntu))

          // Win flavours
          builds.put("VS-v140-x86", stages_win("VS2015-x86","-A x86 -T v140"))
          builds.put("VS-v141-x86", stages_win("VS-v141-x86","-A x86 -T v141"))
          builds.put("VS-v142-x86", stages_win("VS-v142-x86","-A x86 -T v142"))

          builds.put("VS-v140-x64", stages_win("VS-v140-x64","-A x64 -T v140"))
          builds.put("VS-v141-x64", stages_win("VS-v141-x64","-A x64 -T v141"))
          builds.put("VS-v142-x64", stages_win("VS-v142-x64","-A x64 -T v142"))
          
          // all_stages = windows_profiles.collectEntries { pr, labels -> ["${pr}": get_windows_stages(pr, labels)]}
          //all_stages = docker_images.collectEntries { img -> ["${img}": get_linux_stages(img)]}
          parallel(builds)
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