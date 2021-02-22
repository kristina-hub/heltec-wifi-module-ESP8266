from pygooglenews import GoogleNews

gn = GoogleNews()

s = gn.search('boeing OR airbus', when = '1h')

print(s['feed'].title)
# "boeing AND airbus when:1h" - Google News
