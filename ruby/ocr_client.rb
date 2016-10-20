require 'net/http'
module OCR_Recognize
#require 'net/http'
    def get_AreaName()
        uri = URI('https://api.projectoxford.ai/vision/v1.0/ocr')
        image = File.open('image/Nplate.jpg')
        uri.query = URI.encode_www_form({
            # Request parameters
            'language' => 'ja',
            'detectOrientation ' => 'true'
        })
        
        request = Net::HTTP::Post.new(uri.request_uri)
        # Request headers
        request['Content-Type'] = 'application/octet-stream'
        # Request headers
        request['Ocp-Apim-Subscription-Key'] = ENV['AZURE_OCR_API_KEY'] 
        # Request body
        request.body = image.read
        
        response = Net::HTTP.start(uri.host, uri.port, :use_ssl => uri.scheme == 'https') do |http|
            http.request(request)
        end
        #puts response.body
        response.body
    end
    module_function :get_AreaName
end
