
kecho()
{
  echo "[$0:${FUNCNAME[1]}] $@" > /dev/ttyprintk
}

