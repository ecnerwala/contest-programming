if False: print('Make sure to use Python 3 and not Python 2.', end='')

import shutil
import http.server

files = {'/g.git/HEAD': 'g_local.1', '/g.git/info/refs': 'g_local.2', '/g.git/objects/info/packs': 'g_local.3', '/g.git/objects/pack/pack-0f7f574bc0716913f84e74787ed412e94533b1ac.idx': 'g_local.4', '/g.git/objects/pack/pack-0f7f574bc0716913f84e74787ed412e94533b1ac.pack': 'g_local.5'}

class Handler(http.server.BaseHTTPRequestHandler):
    def do_GET(self):
        filename = files.get(self.path.partition('?')[0])
        if not filename:
            self.send_error(404)
            return
        with open(filename, 'rb') as f:
            self.send_response(200)
            self.end_headers()
            shutil.copyfileobj(f, self.wfile)

if __name__ == '__main__':
    print('If everything went well, this script should now be running')
    print('and you should see a line that starts with "Serving HTTP" below.')
    print()
    print('Let this script run, open another terminal and execute the command:')
    print('git clone http://localhost:8000/g.git')
    print()
    http.server.test(HandlerClass=Handler, port=8000)
