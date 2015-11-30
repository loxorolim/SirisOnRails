#require './SirisDevC++/Siris'
require 'os'
class ApplicationController < ActionController::Base


  def autoplan
	toPass = params['data']
	execPath = ""
	path = Dir.pwd;
	if(OS.windows?)
	
		if(OS.bits == 32)
			execPath = path+'/Siris/SirisW32/SirisOnRailsC++.exe'
		end
		if(OS.bits == 64)
			execPath = path+'/Siris/SirisW64/SirisOnRailsC++.exe'
		end
	end
	if(OS.posix?)
		if(OS.bits == 32)	
			execPath = 'LD_LIBRARY_PATH='+path+'/Siris/glnx86/ ' + path+'/Siris/glnx86/SirisOnRailsC++'
		end
		if(OS.bits == 64)
			execPath = 'LD_LIBRARY_PATH='+path+'/Siris/glnxa64/ ' + path+'/Siris/glnxa64/SirisOnRailsC++'
		end
	end

	#print toPass["action_id"]
	#answer = Siris.new.getResponse(toPass, path);
	#teste = RiceTest.new.pathteste(path);
	#print execPath
  	answer = ""
  	#answer2 = ""
	IO.popen(execPath, 'r+') do |pipe|
	  pipe.puts(path)
	  pipe.puts(toPass)	  
	  pipe.close_write
	  answer = pipe.read
	  #answer2 = pipe.read
	  #pipe.close_read
	end
	print answer
	#print answer2

	#print shell_output
	#File.open('teste.txt', 'w') {|f| f.write(toPass)}
	#print toPass

	#system('C:\Sites\first_app\sirisSCPCalculator\SirisSCPCalculator\Release\SirisSCPCalculator.exe C:\Sites\first_app\teste.txt C:\Sites\first_app\teste2.txt');
	#exec('C:\Sites\first_app\sirisSCPCalculator\SirisSCPCalculator\Release\SirisSCPCalculator.exe', toPass);
	#system('C:\Users\Guilherme\Downloads\glpk-4.54\w64\glpsol.exe --math C:\Sites\first_app\teste2.txt');
	#answer = File.read('Results.txt') 
	respond_to do |format|
    msg =  answer 
    format.json  { render :json => msg } # don't do msg.to_json
	format.text  { render :text => msg } # don't do msg.to_json
	end
  end
  def uploadFile
	post = DataFile.save(params[:upload])
	render :text => "File has been uploaded successfully"
  end

 
  # Prevent CSRF attacks by raising an exception.
  # For APIs, you may want to use :null_session instead.
  protect_from_forgery :except => :autoplan
end
