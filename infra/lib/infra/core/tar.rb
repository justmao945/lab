#
# modified from https://gist.github.com/sinisterchipmunk/1335041
#
module Infra

class Tar
  # Creates a tar file in memory recursively
  # from the given path.
  # @return tar data
  def self.tar(path)
    tarfile = StringIO.new("")
    Gem::Package::TarWriter.new(tarfile) do |tar|
      Dir.glob(File.join(path, "**", "*"), File::FNM_DOTMATCH).each do |file|
        next unless File.exist? file
        mode = File.stat(file).mode
        relative_file = file.sub /^#{Regexp::escape path}\/?/, ''

        if File.directory?(file)
          tar.mkdir relative_file, mode
        else
          tar.add_file relative_file, mode do |tf|
            File.open(file, "rb") { |f| tf.write f.read }
          end
        end
      end
    end
    tarfile.string
  end

  # 
  #   {|tarfile| block }
  # @return Hash of tar data {path=> content}
  #         when path is directory, content is nil
  #         when path is file, content is file content
  def self.untar(tar_data)
    io = StringIO.new tar_data
    untar_data = {}
    Gem::Package::TarReader.new io do |tar|
      tar.each do |tarfile|
        yield tarfile if block_given?
        untar_data[tarfile.full_name] = tarfile.directory? ? nil : tarfile.read
      end
    end
    untar_data
  end

  # @return compressed data
  def self.gzip(data)
    gz = StringIO.new("")
    z = Zlib::GzipWriter.new(gz)
    z.write data
    z.close # this is necessary!
    # z was closed to write the gzip footer, so
    # now we need a new StringIO
    gz.string
  end

  # @return decompressed data
  def self.ungzip(gz_data)
    gz = StringIO.new(gz_data)
    z = Zlib::GzipReader.new(gz)
    unzipped = z.read
    z.close
    unzipped
  end

  # compress +path+ to a +file_tar_gz+
  def self.tar_zcf(file_tar_gz, path)
    FileUtils.mkdir_p File.dirname(file_tar_gz)
    System.write gzip(tar(path)), file_tar_gz
  end

  # compress +path+ to +path.tar.gz+ and then delete +path+
  def self.tar_zd(path)
    # clean path
    path = File.join File.dirname(path), File.basename(path)
    # do it then
    tar_zcf "#{path}.tar.gz", path
    FileUtils.rm_rf path
  end

  # extract to disk
  def self.tar_xf(file_tar_gz, path)
    untar(ungzip(File.read(file_tar_gz))) do |f|
      f_path = File.join path, f.full_name
      if f.directory?
        FileUtils.mkdir f_path
      else
        FileUtils.mkdir File.dirname(f_path)
        System.write f.read, f_path
      end
    end
  end

  # extract to memory
  def self.tar_xf_mem(file_tar_gz, &b)
    untar ungzip(File.read(file_tar_gz)), &b
  end
end

end
# vim: set ts=2 sts=2 sw=2:
