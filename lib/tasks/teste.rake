require 'rake/clean'

CLEAN.include('SirisDevC++//**/*{.o,.log,.so,.bundle}')  
CLEAN.include('SirisDevC++//**/Makefile')  
CLOBBER.include('lib/*{.so,.bundle}')

desc 'Build the C extension'  
task :build_ext do  
  Dir.chdir("SirisDevC++/") do
    ruby "extconf.rb"
    sh "make"
  end
end 