import sys
import urllib.parse as urlparse

print("Argumentos recibidos por STDIN: ")

try:
    for line in sys.stdin:
        url = 'foo.com/?' + line
        parsed = urlparse.urlparse(url)

        print('Recibido: {}'.format(urlparse.parse_qs(parsed.query)))
except:
    ignorar = True
