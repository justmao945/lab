# loaded before all tests, do some initializations

require "test/unit"
require 'infra'

include Infra

# the test root path, please place this file in the root
# of test directory.
TEST_ROOT = File.expand_path '../', __FILE__

TEST_DATA = File.join TEST_ROOT, "data"

TEST_LOG_FILE = File.join TEST_ROOT, "test.log"

TEST_TMP_DIR = "/tmp/infra"

FileUtils.rm_rf TEST_TMP_DIR

# global log file
$log_file = File.open TEST_LOG_FILE, "w"

# global logger console
$log = Console.new $log_file

# vim: set ts=2 sts=2 sw=2:
