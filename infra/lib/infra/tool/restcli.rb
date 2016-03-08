module Infra

  class ToolRestCli < ExeContext
    # add options
    def self.init_options(o)
      o.set_opt "-q", false

      o.add_banner "Usage: restcli [option...] <URI>"
      o.add_description <<DES
Simple RESTFul client, only support JOSN/YAML file to post.
DES
      o.add_opt "-x", "--method <get|post|put|delete>", "HTTP request method"
      o.add_opt "-d", "--data <file>", "HTTP request body to post, accept .yaml or .json file"
      o.add_opt "-s", "--save <file>", "HTTP response body received, only .json file"
      o.add_opt "-q", "--quiet", "Do not print received data"
    end

    def run
      raise OptionParser::InvalidArgument, "require <URI>" if @option.args.length != 1
      uri = @option.args.first

      if x = @option.get_opt("-x")
        method = x.to_sym
      else
        method = :get
      end

      case method
      when :get, :delete
        json, res = RestCli.__send__ method, uri
      when :post, :put
        data_file = @option.get_opt "-d"
        raise OptionParser::MissingArgument "-d <file> is requried for method #{method}" unless data_file
        case Mime.new(data_file).type
        when "application/json"
          data = JSON.load File.read data_file
        when "application/x-yaml"
          data = YAML.load_file data_file
        else
          raise OptionParser::InvalidArgument, "invalid data type of #{data_file}"
        end
        json, res = RestCli.__send__ method, uri ,data
      else
        raise OptionParser::InvalidArgument, "invalid method: #{method}"
      end

      if  s = @option.get_opt("-s")
        System.write json.to_pretty, s
      end
      
      if !@option.get_opt("-q")
        @console << :note << "#{res.code} #{res.message}" << :end << "\n" << :flush
        @console << json.to_pretty << "\n" <<  :flush if json
      end

      ! json.nil?
    end 
  end

  Main.add_tool ToolRestCli, "restcli", "simple RESTFul client"

end
# vim: set ft=ruby ts=2 sts=2 sw=2:
