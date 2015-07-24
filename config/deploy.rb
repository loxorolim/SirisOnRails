set :application, "SirisOnRails"
set :repository, "https://github.com/loxorolim/SirisOnRails.git"
set :scm, "git"
set :deploy_to, "/var/www/SirisOnRails"

role :app, "192.168.1.119"
role :web, "192.168.1.119"
role :db, "192.168.1.119", :primary => true
set :user, "guilherme"
set :use_sudo, false
