dnstool
============

[![License](https://img.shields.io/badge/license-Apache%202-4EB1BA.svg)](https://www.apache.org/licenses/LICENSE-2.0.html)


dns performance test tools.


Usage
=============
```
Usage: sudo ./dnstool -s ip [-p port] [-n counts] -l local_ip -d datafile
    -s ip: the server to query
    -p port: the port on which to query the server
    -l ip: the local address from which to send queries
    -n counts: repeatedly the datafile counts times
    -d datafile: the input data file
```

Example:
```
$ cat test.txt
www.google.com.hk

$ sudo ./dnstool -s 192.168.44.128 -l 192.168.44.137 -d test.txt
```

Get Help
============
The fastest way to get response is to send email to my mail <zengxianglong0@gmail.com>

License
============
Please refer to [LICENSE](https://github.com/alandtsang/dnstool/blob/master/LICENSE) file.
