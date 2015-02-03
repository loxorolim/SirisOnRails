require './SirisDevC++/Siris'
class ApplicationController < ActionController::Base


  def autoplan
	toPass = params['data']

	path = Dir.pwd;
	answer = Siris.new.getResponse(toPass, path);
	#teste = RiceTest.new.pathteste(path);
	print answer
  	#answer = ""
  	#answer2 = ""
	#IO.popen('C:\Sites\first_app\sirisSCPCalculator\SirisSCPCalculator\Release\SirisSCPCalculator.exe', 'r+') do |pipe|
	#  pipe.puts(toPass)
	#  pipe.close_write
	#  answer = pipe.read
	#  answer2 = pipe.read
	#  pipe.close_read
	#end
	#print answer
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
