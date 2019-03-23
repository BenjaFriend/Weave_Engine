node {
    //sh 'env > env.txt'
    //for (String i : readFile('env.txt').split("\r?\n")) {
    //    println i
    //}

    sh "echo \$PWD"

    stage('Preparation') { // for display purposes

        // Get some code from a GitHub repository
        git 'https://github.com/BenjaFriend/Weave_Engine.git'
    }

    stage('Build') {
       echo 'Hello I am in the buiiild stage!'
       // Run CMake here, maybe inside a docker container?
       //sh "cmake \$PWD/Weave_Server"
       //sh "make \$PWD/Weave_Server"
    }

    stage('Results') {
        echo 'Ya boi is done'
    }
}
