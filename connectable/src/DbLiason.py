#!/usr/bin/python3
# Author: Emmanuel Odeke <odeke@ualberta.ca>

import os
import sys
import json
import collections

pyVersion = sys.hexversion / (1<<24)

if pyVersion >= 3:
        import urllib.request as urlReqModule
        byteFyer = dict(encoding='utf-8')
else:
        import urllib2 as urlReqModule
        byteFyer = dict()

class DbConn:
    def __init__(self, baseUrl):
        self.baseUrl = baseUrl

    def __urlRequest(self, method, isGet=False, **getData):
        fmtdData = json.dumps(getData)
        reqUrl = self.baseUrl if not isGet else self.baseUrl + '/?' + '&'.join([
            '{k}={v}'.format(k=k, v=v) for k,v in getData.items()
        ])

        req = urlReqModule.Request(reqUrl)
        req.add_header('Content-Type', 'application/json')
        req.get_method = lambda: method.upper()

        dataOut = collections.defaultdict(lambda: b'{}')
        statusCode = 500

        try:
            uR = urlReqModule.urlopen(req, bytes(fmtdData, **byteFyer))
        except Exception as e:
            print(e)
            dataOut['reason'] = e
        else:
            dataOut['value'] = uR.read()
            statusCode = uR.getcode()
        finally:
            dataOut['status_code'] = statusCode

        return dataOut

    def get(self, data):
        return self.__urlRequest('get', isGet=True, **data)

    def put(self, data):
        return self.__urlRequest('put', **data)

    def post(self, data):
        return self.__urlRequest('post', **data)

    def delete(self, data):
        return self.__urlRequest('delete', **data)

class HandlerLiason(object):
    def __init__(self, baseUrl, *args, **kwargs):
        self.baseUrl = baseUrl
        self.__callableCache = collections.defaultdict(
            lambda *arg, **kwargs : 'Not yet defined: {a}, {k}'.format(a=args, k=kwargs)
        )
        self.handler = DbConn(baseUrl)

    def postConn(self, data):
        return self.handler.post(data)

    def deleteConn(self, data):
        return self.handler.delete(data)

    def putConn(self, data):
        return self.handler.put(data)

    def getConn(self, data):
        return self.handler.get(data)

class GCSHandler(object):
    def __init__(self, baseUrl, *args, **kwargs):
        self.baseUrl = baseUrl
        self.__imageHandler = HandlerLiason(baseUrl + '/imageHandler')
        self.__markerHandler = HandlerLiason(baseUrl + '/markerHandler')

    @property
    def imageHandler(self): return self.__imageHandler

    @property
    def markerHandler(self): return self.__markerHandler
