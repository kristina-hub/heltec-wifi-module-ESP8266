import gnewsclient
from gnewsclient import gnewsclient

client = gnewsclient.NewsClient(language='hindi',
                                location='india',
                                topic='Business',
                                max_results=5)

# prints location
print("Location: \n",client.locations)
print()

# prints languages
print("Language \n",client.languages)
print()

# prints topics
print("Topic \n",client.topics) 
