require 'tiny_tds'  

class Azure_DAO
    attr_reader :clinet
    def initialize()
        @client = TinyTds::Client.new username: ENV['SQL_DATABASE_USERNAME'], password: ENV['SQL_DATABASE_PASSWORD'], host: ENV['SQL_DATABASE_HOST']
            
    end

    def get_table()
        sql = "SELECT name FROM sysobjects WHERE xtype = 'U';"
        results = @client.execute(sql)
        results.each do |row|
            puts row 
        end 
 
    end
end
dao = Azure_DAO.new()
dao.get_table
