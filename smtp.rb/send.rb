require 'net/smtp'

from = "test@test.com"
to = "justmao945@gmail.com"

msg = <<EOF

From: demo <#{from}>
To: Jianjun Mao <#{to}>
Subject: SMTP.rb test
Date: #{Time.now}
 
HaHa, this mail is sent by ruby.

EOF

Net::SMTP.start('localhost') do |smtp|
    smtp.send_message msg, from, to
end

