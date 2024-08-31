Here I would like to provide some information about project.

Compile project: g++ -o3 *.cpp -o server -lpthread

In this project I`ve implemented http server using c++ under unix platform.
Server is able to process http GET requests and provide resources which were requested.

Server uses interface: 127.0.0.1:54124 by default.

###########################################################################
ab -c 1020 -n 100000 -l localhost:54124/index.html
This is ApacheBench, Version 2.3 <$Revision: 1843412 $>
Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
Licensed to The Apache Software Foundation, http://www.apache.org/

Benchmarking localhost (be patient)
Completed 10000 requests
Completed 20000 requests
Completed 30000 requests
Completed 40000 requests
Completed 50000 requests
Completed 60000 requests
Completed 70000 requests
Completed 80000 requests
Completed 90000 requests
Completed 100000 requests
Finished 100000 requests


Server Software:        
Server Hostname:        localhost
Server Port:            54124

Document Path:          /index.html
Document Length:        Variable

Concurrency Level:      1020
Time taken for tests:   9.634 seconds
Complete requests:      100000
Failed requests:        0
Total transferred:      115977753 bytes
HTML transferred:       109777753 bytes
Requests per second:    10380.01 [#/sec] (mean)
Time per request:       98.266 [ms] (mean)
Time per request:       0.096 [ms] (mean, across all concurrent requests)
Transfer rate:          11756.35 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0   40   6.6     40      71
Processing:    17   58   9.7     58     100
Waiting:        0   44  10.6     47      79
Total:         50   98   9.3     96     143

Percentage of the requests served within a certain time (ms)
  50%     96
  66%    100
  75%    103
  80%    105
  90%    110
  95%    114
  98%    121
  99%    125
 100%    143 (longest request)
 ###########################################################################
