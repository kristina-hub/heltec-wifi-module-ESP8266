#! python
"""
File: captureNewsTrends.py
Description:
  This script pulls a list of items from the Google News API.  Then it loops through results
  to tally the frequency of key words in the title of each news article.
  The counts for a given date and time are saved into a file.
"""

#import libraries used below
import http.client
import json
from datetime import datetime
from pathlib import Path

# This is where the generated html will be saved (in the local directory)
#  More information about the Path function is described at https://realpython.com/python-pathlib/
data_folder = Path("C:/Users/myUserName/Documents/")
outputFile = data_folder / "newsTrends.csv"

# datetime object containing current date and time
now = datetime.now()

# Get the date and time in the format YYYY-mm-dd H:M:S
dt_string = now.strftime("%Y-%m-%d %H:%M:%S")

# Initialize counters
trumpCnt=0
bidenCnt=0
jorgensenCnt=0
articleCnt=0

# Set to 1 to show details along the way for debugging purposes
debug=0

#This is the url encoded query we will search for in the Google News API
#  Spaces are replaced with %20
query = "US%20Presidential%20Election"

#Connect to the API
conn = http.client.HTTPSConnection("google-news.p.rapidapi.com")

headers = {
    'x-rapidapi-host': "google-news.p.rapidapi.com",
    'x-rapidapi-key': "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
    }

conn.request("GET", "/v1/search?when=1h&country=US&lang=en&q="+query, headers=headers)

res = conn.getresponse()
data = res.read()

# Load API response into a Python dictionary object, encoded as utf-8 string
json_dictionary = json.loads(data.decode("utf-8"))

# Loop through dictionary keys to access each article
for item in json_dictionary['articles']:
    # Pull the title for this article into a variable.
    thisTitle = item['title']
    if debug>0:
        print("Title:", thisTitle)

  #Get a count of keywords in the article title
    trumpCnt+= thisTitle.upper().count("TRUMP")
    bidenCnt+= thisTitle.upper().count("BIDEN")
    jorgensenCnt+= thisTitle.upper().count("JORGENSEN")
    articleCnt+=1

# Create summary line for the csv file
outputCSV = str(dt_string) + "," + str(articleCnt) + "," +  str(trumpCnt) + "," + str(bidenCnt) + "," + str(jorgensenCnt) + "\n"

#Now populate the csv file
with open(outputFile, "a", encoding="utf-8") as f:
    f.write(outputCSV)
