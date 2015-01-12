require 'rake/clean'

CLEAN.include('SirisDevC++/**/*{.o,.log,.so,.bundle}')  
CLEAN.include('SirisDevC++/**/Makefile')  


desc 'Build the C++ extension'  
task :build_siris do  
  Dir.chdir("SirisDevC++/") do     
    ruby "extconf.rb"
    sh "make"
  end
end 