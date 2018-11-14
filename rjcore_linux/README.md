
## Env
1. mkdir /usr/local/java
2. cd /usr/local/java
3. tar zxvf jdk-8u192-linux-x64.tar.gz
4. cp jna-5.0.0.jar /usr/local/java/jdk1.8.0_192/lib
5. vi /etc/profile    # add the comment in the end of line
```
export JAVA_HOME=/usr/local/java/jdk1.8.0_192
export JRE_HOME=${JAVA_HOME}/jre
export CLASSPATH=.:${JAVA_HOME}/lib:${JRE_HOME}/lib:$CLASSPATH:${JAVA_HOME}/lib/jna-5.0.0.jar:/usr/local/lib/sysrjcore_linux.jar
export JAVA_PATH=${JAVA_HOME}/bin:${JRE_HOME}/bin
export PATH=$PATH:${JAVA_PATH}
```

## Build
1. make && make install && ldconfig


