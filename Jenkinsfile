node {

    sh "echo \$PWD"

    stage('Preparation') { // for display purposes
        // Get some code from a GitHub repository
        git 'https://github.com/BenjaFriend/Weave_Engine.git'
        sh "git submodule update --init --recursive;"
    }

    stage('Build Server GCC') {
       echo 'Building with GCC...'
       sh "g++ --version"
       // Run CMake here, maybe inside a docker container?
       sh "cmake \$PWD/Weave_Server -DUSE_CLANG=OFF"
       sh "make \$PWD/Weave_Server"
    }

    stage('Build Server Clang') {
       echo 'Building with Clang...'
       sh "clang++ --version"
       // Run CMake here, maybe inside a docker container?
       sh "cmake \$PWD/Weave_Server -DUSE_CLANG=ON"
       sh "make \$PWD/Weave_Server"
    }

    stage('Results') {
        echo 'Ya boi is done'
    }
}
