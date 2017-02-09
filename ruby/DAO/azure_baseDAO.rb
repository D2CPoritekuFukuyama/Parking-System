require 'tiny_tds'  

class Azure_DAO
    attr_reader :clinet
    def initialize()
        @client = TinyTds::Client.new username: ENV['SQL_DATABASE_USERNAME'], password: ENV['SQL_DATABASE_PASSWORD'], dataserver: ENV['SQL_DATABASE_HOST'],database: ENV['SQL_DATABASE_NAME'], login_timeout: '5'
            
    end

    def get_table()
        sql = "SELECT * FROM dbo.会員;"
        results = @client.execute(sql)
        results.each do |row|
            puts row 
        end 
 
    end
end
dao = Azure_DAO.new()
dao.get_table
