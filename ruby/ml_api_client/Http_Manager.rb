require 'net/http'

class Http_Manager
    def initialize(url, api_key)
        @uri = URI(url)
        @request = Net::HTTP::Post.new(@uri.request_uri)
        @request['Content-Type'] = 'application/json' 
        @request['Authorization'] = 'Bearer ' + ENV[api_key] 
    end
end
