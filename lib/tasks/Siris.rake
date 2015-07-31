require 'rake/clean'

CLEAN.include('SirisDevC++/**/*{.o,.log,.so,.bundle,.def}')  
CLEAN.include('SirisDevC++/**/Makefile')  


desc 'Build the C++ extension'  
task :build_siris => [:clean] do
  Dir.chdir("SirisDevC++/") do   
    ruby "extconf.rb"
    sh "make"
  end
end 
task :build_siris_w32 => [:clean] do
  Dir.chdir("SirisDevC++/") do   
    ruby "extconfw32.rb"
    sh "make"
  end
end 
task :build_siris_w64 => [:clean] do
  Dir.chdir("SirisDevC++/") do   
    ruby "extconfw64.rb"
    sh "make"
  end
end 