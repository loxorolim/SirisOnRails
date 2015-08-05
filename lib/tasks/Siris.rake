require 'rake/clean'

CLEAN.include('SirisDevC++/**/*{.o,.log,.so,.bundle,.def}')  
CLEAN.include('SirisDevC++/**/Makefile')  


desc 'Build the C++ extension'  
task :build_siris_linux64 => [:clean] do
  Dir.chdir("SirisDevC++/") do   
    system("g++ Siris.cpp AutoPlanningMethods.cpp auxiliar.cpp Grid.cpp HataSRD.cpp HeatgridMethods.cpp LinkCalculationMethods.cpp  MetricCalculationMethods.cpp TestMethods.cpp -I GLPK -I VLFEAT -L GLPK/lib -L VLFEAT/bin/glnxa64 -lglpk -lvl -std=c++11 -o ../Siris/glnxa64/siris")
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