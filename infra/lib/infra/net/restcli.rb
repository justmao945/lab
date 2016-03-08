module Infra

class RestCli
  
  def self.get(url)
    request Net::HTTP::Get.new URI url
  end

  def self.post(url, data)
    request Net::HTTP::Post.new(URI url), data
  end

  def self.put(url, data)
    request Net::HTTP::Put.new(URI url), data
  end

  def self.delete(url)
    request Net::HTTP::Delete.new URI url
  end

  # @return [JSON, HTTPResponse]
  def self.request(req, data=nil)
    mime_type = "application/json"
    req.add_field 'Accept', mime_type
    if data
      req.content_type = mime_type
      req.body = data.to_json
    end
    uri = req.uri
    res = Net::HTTP.start uri.hostname, uri.port do |http|
      http.request req
    end
    if res.content_type == mime_type
      [JSON.load(res.body), res]
    else
      [nil, res]
    end
  end
end

end
# vim: set ts=2 sts=2 sw=2:
