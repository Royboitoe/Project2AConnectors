import pandas as pd
import numpy as np
import random

data = pd.read_csv("UNCLEANED Melbourne Dataset.csv")
data.columns = ['Suburb', 'Address', 'Rooms', 'Type', 'Price', 'Postcode', 'Bedrooms',
        'Bathrooms', 'LandSize', 'BuildingArea', 'YearBuilt', 'CouncilArea',
       'Region', 'PropertyCount']

# Uniqueness check
def uniqueness_check():
    print("\n---------Uniqueness Checks--------")
    for i in data.columns:
        if pd.unique(data[i]).shape[0] != data.shape[0]:
            print(i, "Uniqueness : 0")
        else:
            print(i, "Uniqueness : 1")

# Null check
def null_check():
    print("\n-----------Null Checks------------")
    for j in data.columns:
        null_count = data[pd.isnull(data[j])].shape[0]
        print(j, "null value count:", null_count)

uniqueness_check()
null_check()

# ==> We intend to make price unique by randomizing in its current range
# ==> Before this we will clean data in the remaining columns

# Cleaning numerical columns
fix_cols = ['Postcode', 'Bedrooms', 'Bathrooms', 'LandSize', 'BuildingArea' , 'YearBuilt', 'PropertyCount', 'CouncilArea', 'Region']

for i in fix_cols:
    if i == 'CouncilArea' or i == 'Region':
        most_common = data[i].mode(dropna=True)[0]
        data[i] = data[i].fillna(most_common)
        continue

    avg = data[i].mean()
    data[i] = data[i].replace(0, avg)

    # col_min = data[i].min()
    # col_max = data[i].max()
    sd = data[i].std()
    filler = abs(random.gauss(mu = avg, sigma=sd/12))

    data[i] = data[i].fillna(filler)
    data[i] = data[i].astype('int')


# ------------Price Fixing------------
price_min = data.Price.min()
price_max = data.Price.max()

# ==> random.sample picks w/o replacement
new_prices = pd.Series(random.sample(population = list(np.arange(price_min, price_max)), k = data.shape[0]))

# while pd.unique(new_prices).shape[0] != data.shape[0]:
#     new_prices = pd.Series([int(random.uniform(a = price_min, b = price_max)) for i in range(data.shape[0])])

data.Price = new_prices.astype('int')
uniqueness_check()
null_check()

data.to_csv("cleaned_pt2.csv", index = False)