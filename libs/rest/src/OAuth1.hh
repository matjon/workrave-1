// Copyright (C) 2010 - 2013 by Rob Caelers <robc@krandor.nl>
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef OAUTH1_HH
#define OAUTH1_HH

#include <string>
#include <map>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#include "rest/IOAuth1.hh"
#include "rest/IHttpSession.hh"

#include "OAuth1Filter.hh"

class OAuth1 : public workrave::rest::IOAuth1
{
public:
  typedef boost::shared_ptr<OAuth1> Ptr;

public:
  static Ptr create(const Settings &settings);

 	OAuth1(const Settings &settings);
  virtual ~OAuth1();
  
  void init(const std::string &consumer_key,
            const std::string &consumer_secret,
            SuccessCallback success_cb,
            FailedCallback failure_cb);

private:
  void request_temporary_credentials();
  void request_resource_owner_authorization();
  void request_token(const std::string &verifier);

  void on_temporary_credentials_ready(workrave::rest::IHttpReply::Ptr reply);
  workrave::rest::IHttpReply::Ptr on_resource_owner_authorization_ready(workrave::rest::IHttpRequest::Ptr request);
  void on_token_ready(workrave::rest::IHttpReply::Ptr reply);

  workrave::rest::IHttpRequestFilter::Ptr create_filter();
  
  void parse_query(const std::string &query, OAuth1Filter::RequestParams &params) const;

  void failure();
  void success();
  
private:  
  workrave::rest::IHttpSession::Ptr session;
  workrave::rest::IHttpServer::Ptr server;
  OAuth1Filter::Ptr filter;
  
  Settings settings;

  std::string token;
  std::string oauth_callback;
  
  SuccessCallback success_cb;
  FailedCallback failure_cb;
  
  std::string verified_path;
};

#endif
