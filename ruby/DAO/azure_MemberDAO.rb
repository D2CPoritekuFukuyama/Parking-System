require 'tiny_tds'  
require './DAO/azure_baseDAO.rb'

class Member_DAO < Azure_DAO 
    def get_member(number, cate_num, hiragana, area)
#        sql = "SELECT 会員ID,PLATENo, Area, hiragana 
#                FROM 会員"
        sql = "SELECT id,PLATENo1, Area, hiragana 
                FROM dbo.会員 
                WHERE PLATENo1 = '#{number.to_i}'
                or Area = (N'#{area.sub(/\s/,"")}')
                AND hiragana = '#{hiragana}';" 
        results = @client.execute(sql)
#        results.each do |row|
#            puts row
#        end
        return results
    end
end

#member = Member_DAO.new
#results = member.get_member("1122", "300", "to", "倉敷")
#results.each do |row|
#    puts row
#end
