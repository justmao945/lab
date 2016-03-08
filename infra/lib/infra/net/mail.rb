module Infra

class Contact
  attr_reader :name, :mail
  def initialize(name, mail)
    @name = name
    @mail = mail
  end
end

class Mail
  def initialize(opts)
    @server = opts[:server] || 'localhost'
    @port = opts[:port] || 25

    # Contact
    @from = opts[:from]
    # Array of Contact
    @to_list = opts[:to_list] || []
    @cc_list = opts[:cc_list] || []

    @subject = opts[:subject]
    @date    = opts[:date] || Time.now
    @body    = opts[:body]

    @doc = "FROM: #{@from.name} <#{@from.mail}>\r\n"
    @to_list.each{|c| @doc << "TO: #{c.name} <#{c.mail}>\r\n"}
    @cc_list.each{|c| @doc << "CC: #{c.name} <#{c.mail}>\r\n"}
    @doc << "Subject: #{@subject}\r\nDate: #{@date}\r\n\r\n\r\n#{@body}\r\n"
  end

  def send
    e = nil
    send_to = (@to_list + @cc_list).map{|c| c.mail}
    begin
      Net::SMTP.start(@server, @port) do |smtp|
        smtp.send_message @doc, @from.mail, send_to
      end
    rescue Exception=> e
    end
    e
  end

end

end
# vim: set ts=2 sts=2 sw=2:
