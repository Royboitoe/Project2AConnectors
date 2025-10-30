import pandas as pd
import random

data = pd.read_csv("UNCLEANED Melbourne Dataset.csv")
old_cols = ['Suburb', 'Address', 'Rooms', 'Type', 'Price', 'Postcode', 'Bedrooms',
        'Bathrooms', 'LandSize', 'BuildingArea', 'YearBuilt', 'CouncilArea',
       'Region', 'PropertyCount']

data.columns = old_cols

fix_cols = ['Price', 'Postcode', 'Bedrooms', 'Bathrooms', 'LandSize', 'BuildingArea' , 'YearBuilt', 'PropertyCount']

for i in fix_cols:
    avg = data[i].mean()
    st_dev = data[i].std()
    filler = abs(random.gauss(avg, st_dev / 2))

    if i == "LandSize" or i == "Bathrooms" or i == "BuildingArea":
        data[i].replace(0, filler, inplace=True)

    data[i].fillna(filler, inplace=True)
    data[i] = data[i].astype('int')

new_cols = ['Id'] + old_cols

ids = pd.Series()
while pd.unique(ids).shape[0] != data.shape[0]:
    ids = pd.Series([int(random.uniform(100000000, 999999999)) for i in range(data.shape[0])])

data['Id'] = ids
data = data[new_cols]
data.to_csv("cleaned.csv", index = False)