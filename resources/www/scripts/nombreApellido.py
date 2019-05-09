import sys
import urllib.parse as urlparse

web_code = '<html><head><p>Bienvenidx <font size=15>{} {}</p></font></head></html>'

try:
    for line in sys.stdin:
        url = 'foo.com/?' + line
        parsed = urlparse.urlparse(url)

        nombre = urlparse.parse_qs(parsed.query)['nombre'][0]
        apellido = urlparse.parse_qs(parsed.query)['apellido'][0]

        print(web_code.format(nombre, apellido))
except:
    print('Error en los campos de nombre o apellido')
