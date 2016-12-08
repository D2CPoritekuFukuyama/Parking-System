require 'tiny_tds'  
require './DAO/azure_baseDAO.rb'

class Member_DAO < Azure_DAO 
    def get_member(number, cate_num, hiragana, area)
        sql = "SELECT 会員ID,PLATENo, Category, Area, hiragana 
                FROM 会員 
                AND PLATENo = '#{number.to_i}'
                AND Area = (N'#{area.sub(/\s/,"")}')
                AND hiragana = 'hiragana';" 
        results = @client.execute(sql)
#        results.each do |row|
#            puts row
#        end
        return results
    end
end

