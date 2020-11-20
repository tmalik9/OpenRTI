artifactoryServer = Artifactory.server 'vistrpndart1'
artifactoryBuildInfo = Artifactory.newBuildInfo()

def checkout() {
  stage("Checkout") {
    def checkoutResults = checkout scm
    //echo '-----> checkout results:\n' + checkoutResults.toString()
    env.isTag = checkoutResults.SVN_URL.contains('tags')
    if (env.isTag == "true") {
      env.tagNr = checkoutResults.SVN_URL.tokenize('/').last()
    } else {
      env.tagNr = "OFF"
    }
    //echo '-----> env:\n' + sh(script: 'env|sort', returnStdout: true)
    //echo "-----> Checkout: isTag ${env.isTag} tagNr ${env.tagNr}"
  }
}

def build_linux(img, configType, additionalCmakeArgs) {
    def buildDir = "build_${configType}"
    stage("Build ${configType}") {
        sh "mkdir ${buildDir}"
        sh "chmod +x build.sh"
        dir(buildDir) {
          def build_cmd = "../build.sh ${configType} ${env.tagNr} ${additionalCmakeArgs}"
          if (img.contains('centos')) {
            sh "scl enable llvm-toolset-6.0 '${build_cmd}'"
          } else {
            sh "${build_cmd}"
          }
        }
    }
}

def test_linux(configType) {
	def buildDir = "build_${configType}"
    stage("Test ${configType}") {
        dir(buildDir) {
            sh "ctest --verbose --build-config ${configType}"
        }
    }
}

def stages_linux(build_name, comp_env, img, deploy) {
  return {
    stage(build_name) {
      node('docker && linux') {
        withEnv(comp_env) {
          try 
          {
            checkout()

            docker.withRegistry('https://pnd-rtklinux-docker-dev.vegistry.vg.vector.int/', 'fa92756a-62a2-4436-9a66-ceb0c2c109a2') {
              docker.image(img).inside {

				def additionalCmakeArgs = ""
				if (env.isTag == "false") {
					additionalCmakeArgs = "-DOPENRTI_ENABLE_TESTS=ON"
				}

                build_linux(img, "debug", additionalCmakeArgs)
				if (env.isTag == "false") {
					test_linux("debug")
				}
				
                build_linux(img, "release" , additionalCmakeArgs)
                if (env.isTag == "false") {
					test_linux("release")
				}
				
                if (env.isTag == "true" && deploy) {
                  upload("build_*/*.tgz")
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

def build_win(configType, additionalCmakeArgs) {
    def buildDir = "build_${configType}"
    stage("Build ${configType}") {
      cmakeBuild(
          installation: 'InSearchPath', 
          buildDir: "${buildDir}",
          buildType: "${configType}",
          generator: "Visual Studio 16 2019",
          cleanBuild: true,
          cmakeArgs: "${additionalCmakeArgs} -DCMAKE_BUILD_TYPE=${configType}",
          steps: [[
              args: "--config ${configType}",
              withCmake: true
          ]]
      )
    }
}

def test_win(configType) {
	def buildDir = "build_${configType}"
    stage("Test ${configType}") {
      ctest(
          installation: 'InSearchPath',
		  workingDir: "${buildDir}",
          arguments: "--verbose --build-config ${configType}"
      )
    }
}

def stages_win(build_name, additionalCmakeArgs, deploy) {
  return {
    stage(build_name) {
      node('buildtools2019') {
        withEnv([]) {
          try 
          {
            checkout()

			if (env.isTag == "false") {
				additionalCmakeArgs = "${additionalCmakeArgs} -DOPENRTI_ENABLE_TESTS=ON"
			}
				
            build_win("debug", additionalCmakeArgs)
			if (env.isTag == "false") {
			    test_win("debug")
			}
			
            build_win("release", additionalCmakeArgs)
            if (env.isTag == "false") {
				test_win("release")
			}
			
            if (env.isTag == "true" && deploy) {
                
              def zipFileName_debug = "openrti-${env.tagNr}-${build_name}-debug.zip"
              zip zipFile: zipFileName_debug, archive: false, dir: 'build_debug/bin/Debug/', glob: '*.dll,*.exe,*.pdb'
              
              def zipFileName_release = "openrti-${env.tagNr}-${build_name}-release.zip"
              zip zipFile: zipFileName_release, archive: false, dir: 'build_release/bin/Release/', glob: '*.dll,*.exe'

              upload("openrti-*.zip")
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

def upload(pattern) {
  def tagNrShort = env.tagNr.substring(0,env.tagNr.lastIndexOf('.'))
  stage('Deploy') {
    artifactoryServer.upload buildInfo: artifactoryBuildInfo, spec: """{
          "files": [
              {
              "pattern": "${pattern}",
              "target": "pnd-rtklinux-generic-dev/OpenRTI/upload/${tagNrShort}/${env.tagNr}/",
              "flat" : "true"
              }
          ]
      }"""
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

          env.mailTo = "V-VGPNDVATDistSim@vector.com"
          //env.mailTo = "Konrad.Breitsprecher@vector.com"

          def builds = [:]

          // Linux flavours
          env_clang = ["CC=clang","CXX=clang++"]
          env_gcc = ["CC=gcc","CXX=g++"]
          docker_image_centos = "pnd-rtklinux-docker-dev.vegistry.vg.vector.int/pnd-rtklinux-build-centos7:1.4"
          docker_image_ubuntu = "pnd-rtklinux-docker-dev.vegistry.vg.vector.int/pnd-rtklinux-build-ubuntu1804:1.1"

          builds.put("centos7 clang", stages_linux("centos7 clang", env_clang, docker_image_centos, true))
          //builds.put("ubuntu1804 clang", stages_linux("ubuntu1804 clang", env_clang, docker_image_ubuntu))
          builds.put("ubuntu1804 gcc", stages_linux("ubuntu1804 gcc", env_gcc, docker_image_ubuntu, true))

          // Win flavours
          //builds.put("VS-v140-x86", stages_win("VS-v140-x86","-A Win32 -T v140", false))
          builds.put("VS-v141-x86", stages_win("VS-v141-x86","-A Win32 -T v141", true))
          //builds.put("VS-v142-x86", stages_win("VS-v142-x86","-A Win32 -T v142", false))

          //builds.put("VS-v140-x64", stages_win("VS-v140-x64","-A x64 -T v140", false))
          builds.put("VS-v141-x64", stages_win("VS-v141-x64","-A x64 -T v141", true))
          //builds.put("VS-v142-x64", stages_win("VS-v142-x64","-A x64 -T v142", false))

          parallel(builds)
        }
      }
    }
  }
  post {
    failure {
      emailext ( subject: '[Jenkins OpenRTI] OpenRTI Build Failed',
                 mimeType: 'text/html',
                 to: "${env.mailTo}",
                 recipientProviders: [[$class: 'CulpritsRecipientProvider']],
                 body: "The OpenRTI Build failed:  ${env.RUN_DISPLAY_URL} <br/> Please check if you are responsible for the build break.")
    }
    success {
      emailext ( subject: '[Jenkins OpenRTI] OpenRTI Build Succeeded',
                  mimeType: 'text/html',
                  to: "${env.mailTo}",
                  recipientProviders: [[$class: 'CulpritsRecipientProvider']],
                  body: "The OpenRTI Build succeeded:  ${env.RUN_DISPLAY_URL}")
    }
  }
}