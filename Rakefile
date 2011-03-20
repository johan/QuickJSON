require 'etc'
require 'json'
require 'hpricot'
require 'shellwords'

SubstVars = %w(DEV_MODE VERSION GIT_HASH HTML_HEADER HTML_FOOTER)
#ENV['VERSION']   = IO.read('VERSION').chomp
ENV["GIT_HASH"]  = IO.popen("git log --format=%H -n 1") { |f| f.gets.chomp }

# not used yet
#if ENV['DEV'] then
#  ENV['DEV_MODE'] ||= 'true'
#else
#  ENV['DEV_MODE'] ||= 'false'
#end

task :default => 'QuickJSON/quicklookjson.c'

desc 'build everything'
task :all => %w(
               QuickJSON/quicklookjson.c
             )

desc 'cleanup'
task :clean do |t|
  #verbose(true) { rm_rf FileList['build/*'] }
end

desc 'Generates the quicklook source code from the test web page.'
file 'QuickJSON/quicklookjson.c' => FileList['json-viewer/quicklook.*'] do |t|
  announce t

  # grab the leading and trailing portions and escape them to C strings
  quicklook_html =~ /\A(.*)__JSON__(.*)\Z/m
  ENV['HTML_HEADER'] = $1.to_json
  ENV['HTML_FOOTER'] = $2.to_json

  # move our C file template into place and perform __MAGIC__ expansion on it
  cp 'json-viewer/quicklook.c', t.name
  write_file t.name
end

# fetch the html, suck out whitespace, inline and minify scripts and stylesheets
def quicklook_html
  cd 'json-viewer' do
    quicklook_html = IO.read('quicklook.html')
    dom = Hpricot.parse(quicklook_html)

    # strip leading/trailing ws in text nodes, and consecutive mid-textnode ws
    dom.search("*").each do |node|
      node.content = node.content().strip.gsub(/\s+/, ' ') if node.text?
    end

    # inline and minify linked stylesheets
    dom.search('link[@rel="stylesheet"][@href]').each do |link|
      path = link.attributes['href']
      css = IO.popen("../bin/cssmin #{Shellwords.escape(path)}") { |s| s.read }
      link.swap("<style>#{css}</style>")
    end

    # inline script tags with minified versions of their content
    dom.search('script[@src]').each do |script|
      src = script.attributes['src']
      script.remove_attribute('src')
      script.inner_html =
        IO.popen("../bin/jsmin #{Shellwords.escape(src)}") do |stdout|
          stdout.read.gsub(/\n/, ' ')
        end
    end
    return dom.to_s
  end
end

def announce(task, action = 'building')
  puts "#{action} #{task.name}"
end

# edit a file in place, replacing __FOO__ any occurrences with ENV['FOO']
def write_file(file, str = nil, no_subst = false)
  str = IO.read(file) unless str
  when_writing do
    File.open(file, 'w') {|f| f.write(no_subst ? str : vars_subst(str)) }
  end
end

$blabbed = {}
def vars_subst(str)
  str = str.dup
  for var in SubstVars
    if ENV[var] && str.gsub!("__#{var}__") { ENV[var] } &&
        ($blabbed[var] || '') != ENV[var]
      show = truncate_if_longer_than(ENV[var], 70 - var.length)
      puts "#{var} is #{show}"
      $blabbed[var] = ENV[var]
    end
  end
  str
end

def truncate_if_longer_than(what, max)
  if (what.size <= max+10) then
    what
  else
    /\A([^\n]{0,20}\b).*?(\b\S[^\n]{0,20})\Z/m.match(what) || # tries to
      /\A([^\n]{0,20}\b).*?(\S[^\n]{0,20})\Z/m.match(what) || # truncate
      /\A([^\n]{0,20}).*?(\b\S[^\n]{0,20})\Z/m.match(what) || # to word
        /\A([^\n]{0,20}).*?(\S[^\n]{0,20})\Z/m.match(what)    # boundary
    $1 +
      ' ... ' + nice_bytes(what.length - $1.length - $2.length, 2) +
      ' ... ' + $2
  end
end

# poached from Ramaze::Helper::NiceBytes
K = 2.0**10
M = 2.0**20
G = 2.0**30
T = 2.0**40
def nice_bytes(bytes, max_digits=3)
  value, suffix, precision = case bytes
    when 0...K
      [ bytes, 'b', 0 ]
    else
      value, suffix = case bytes
        when K...M : [ bytes / K, 'KiB' ]
        when M...G : [ bytes / M, 'MiB' ]
        when G...T : [ bytes / G, 'GiB' ]
        else         [ bytes / T, 'TiB' ]
      end
      used_digits = case value
        when   0...10   : 1
        when  10...100  : 2
        when 100...1000 : 3
      end
      leftover_digits = max_digits - used_digits
      [ value, suffix, leftover_digits > 0 ? leftover_digits : 0 ]
  end
  "%.#{precision}f#{suffix}" % value
end
