node {

    sh "echo \$PWD"

    stage('Preparation') { 
        // Get some code from a GitHub repository
        git 'https://github.com/BenjaFriend/Weave_Engine.git'
        sh "git submodule update --init --recursive;"
    }

    stage('Build Server GCC') {
       echo 'Building with GCC...'
       sh "g++ --version"
       sh "cmake \$PWD -DUSE_CLANG=OFF"
       sh "make -C \$PWD"
    }

    stage('Unit Tests GCC') {
        sh "g++ --version"
        //sh "cmake \$PWD/Weave_Tests -DUSE_CLANG=OFF"
        //sh "make -C \$PWD/Weave_Tests"
        
        echo 'Run Unit tests...'
        sh "./Weave_Tests/bin/WEAVE_TESTS"
    }

    stage('Build Server Clang') {
       echo 'Building with Clang...'
       sh "clang++ --version"

       sh "cmake \$PWD -DUSE_CLANG=ON"
       sh "make -C \$PWD"
    }

    stage('Unit Tests Clang') {
        sh "clang++ --version"

        //sh "cmake \$PWD/Weave_Tests -DUSE_CLANG=ON"
        //sh "make -C \$PWD/Weave_Tests"
        
        echo 'Run Unit tests...'
        sh "./Weave_Tests/bin/WEAVE_TESTS"
    }


}
