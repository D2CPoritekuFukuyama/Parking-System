require 'tiny_tds'  
require './DAO/azure_baseDAO.rb'

class Member_DAO < Azure_DAO 
    def get_member(number, cate_num, hiragana, area)
#        sql = "SELECT 会員ID,PLATENo, Area, hiragana 
#                FROM 会員"
#        puts "hiragana:#{hiragana}"
        sql = "SELECT Nplate_ID,number, area, hiragana 
                FROM members_Nplate
                WHERE number = '#{number.to_i}'
                AND Area = (N'#{area.sub(/\s/,"")}')
                AND hiragana = '#{hiragana}';" 
        results = @client.execute(sql)
#        results.each do |row|
#            puts row
#        end
        return results
    end
end

#member = Member_DAO.new
#results = member.get_member("2456", "301", "to", "福山")
#results.each do |row|
#    puts row
#end
