require 'net/http'
module OCR_Recognize
#require 'net/http'
    def get_AreaName()
        uri = URI('https://api.projectoxford.ai/vision/v1.0/ocr')
        image = File.open('image/Nplate-up1.jpg')
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
    
    def parse_json(json_data)
        begin
            json =  JSON.parse(json_data) 
            regions = json["regions"]
            region = regions[0]
            lines = region["lines"]    
            line = lines[0]
            words = line["words"]
        rescue
            return nil
        end
        text = ""
        words.each do |word|
            text << word["text"]
        end 
        result = ""
        text.chars do |str|
            #if(str =~ /[^ -~｡-ﾟ]/)
            if(str =~ /\p{hiragana}|[ー-龠]/)
                result << str 
            end 
        end 
        #puts result 
        #puts stdArray[0]
        return result
    end
    module_function :parse_json
end
