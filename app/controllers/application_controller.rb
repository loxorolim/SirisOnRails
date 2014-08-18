class ApplicationController < ActionController::Base


  def autoplan
	
	File.open('teste.txt', 'w') {|f| f.write(params['data'])}
	system('C:\Users\Guilherme\Downloads\glpk-4.54\w64\glpsol.exe --math C:\Sites\first_app\teste.txt');
	answer = File.read('Results.txt') 
	respond_to do |format|
    msg =  answer 
    format.json  { render :json => msg } # don't do msg.to_json
	format.text  { render :text => msg } # don't do msg.to_json
	end
  end
  # Prevent CSRF attacks by raising an exception.
  # For APIs, you may want to use :null_session instead.
  protect_from_forgery :except => :autoplan
end
