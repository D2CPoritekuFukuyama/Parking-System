require 'tiny_tds'  

class Azure_DAO
    attr_reader :clinet
    def initialize()
        @client = TinyTds::Client.new( 
            username: "#{ENV['SQL_DATABASE_USERID']}@#{ENV['SQL_DATABASE_SERVER']}.database.windows.net", 
            password: ENV['SQL_DATABASE_PASSWORD'],  
            host: "#{ENV['SQL_DATABASE_SERVER']}.database.windows.net", port: 1433,  
            database: "#{ENV['SQL_DATABASE_NAME']}", azure: true)
    end

    def get_table()
        sql = "SELECT * FROM dbo.会員;"
        results = @client.execute(sql)
        results.each do |row|
            puts row 
        end 
 
    end
end

test = Azure_DAO.new
test.get_table
