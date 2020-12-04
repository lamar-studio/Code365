#!/bin/bash

username=$1
password=$2
serverIp="http://172.30.44.26"

curl www.baidu.com > portal.tmp 2>&1
url1=$(grep -rn NextURL portal.tmp | awk -F'>' '{print $2}' |awk '{print $1}')
url2=$(curl $url1| awk -F"'" '{print $2}')
url2="$serverIp$url2"
curl -v $url2 > portal.tmp 2>&1
sessionId=$(grep sessionId= portal.tmp | awk -F"=" '{print $2}' | sed 's/.$//')

curl $serverIp"/api/auth/fixedAccount" --data-binary '{"account":"'$username'","password":"'$password'","sessionId":"'$sessionId'"}' -H 'Content-Type: application/json;' >  portal.tmp 2>&1

logon_url=$(cat portal.tmp | awk -F "logonUrl" '{print $2}' | awk -F'"' '{print $3}')

curl $logon_url 2>&1

ping www.baidu.com

