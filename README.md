# Agenda

2018 Sun-yat Sen University software enginnering training project.

2018 年中山大学软件工程初级实训项目

Backend: C++

Web Backend: Django (Calling backend via Python extension in C++)

Web Frontend: React

**Live demo:** [https://agenda.howardlau.me](https://agenda.howardlau.me)

### Screenshot
![Agenda](https://howardlau.me/wp-content/uploads/2018/11/Screenshot_20181117_151936.png)

# Build and Run

For CLI: 
```bash
make clean && make
./bin/main
```

For Web (For development):

It is suggested that `nvm` be installed, run this command
```bash
curl -o- https://raw.githubusercontent.com/creationix/nvm/v0.33.11/install.sh | bash
```

After installation, run
```bash
nvm install node
```

If you are in China, please configure the mirrors to speedup installation: 
```bash
pip install pip -U
pip config set global.index-url https://pypi.tuna.tsinghua.edu.cn/simple
npm install -g cnpm --registry=https://registry.npm.taobao.org
``` 
Then you can run the development environment by running:
```bash
pip install . # this installs the pyagenda module
pip install -r requirements.txt
cd webagenda
python manage.py runserver &
cd frontend
cnpm install # or npm install
npm run start
```

# Deployment

First change the api base url in `webagenda/frontend/src/index.js` to your own domain. Then run this command:

```bash
npm run build
```

Put the generated `build` dir wherever you like. Then setup a web server (e.g. `nginx`) to serve the static files.

Next, `cd` into `webagenda` and run this command to start a `uwsgi` applicaiton:

```bash
uwsgi --socket webagenda.sock --module webagenda.wsgi --chmod-socket=666 & 
```

Configure the server to pass api requests to `uwsgi` application, here is an example config for `nginx`:

```nginx
upstream agenda {
    server unix:///path/to/webagenda/webagenda.sock;
}

server {
    listen 443 ssl http2 ;
    listen [::]:443 ssl http2;
    ssl on;
    ssl_certificate /path/to/your.crt;
    ssl_certificate_key /path/to/your.key;
    server_name your.domain;
    root /path/to/your/build;
    index index.html index.htm;
    location / {
        try_files $uri /index.html;
    }
    location /api/v1 {
        uwsgi_pass  agenda;
        include uwsgi_params;
    }
}
```

Make sure your new configuration takes effects, then open your browser and point to your domain. That's all.
