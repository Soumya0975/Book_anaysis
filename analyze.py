import pandas as pd
import matplotlib.pyplot as plt
import os

# Load data
df = pd.read_csv("books.csv")
os.makedirs("charts", exist_ok=True)

# Overview
print("="*50)
print("📚 BOOKS DATASET OVERVIEW")
print("="*50)
print(f"Total Books    : {len(df)}")
print(f"Years Covered  : {df['Year'].min()} - {df['Year'].max()}")
print(f"Average Rating : {df['User Rating'].mean():.2f}")
print(f"Average Price  : ${df['Price'].mean():.2f}")

# Analysis
print("\n📊 TOP 5 HIGHEST RATED:")
print(df.nlargest(5,'User Rating')[['Name','User Rating','Year']].to_string(index=False))

print("\n📊 TOP 5 MOST REVIEWED:")
print(df.nlargest(5,'Reviews')[['Name','Reviews']].to_string(index=False))

print("\n📊 AVERAGE RATING BY GENRE:")
print(df.groupby('Genre')['User Rating'].mean().round(2).to_string())

print("\n📊 2024 BOOKS:")
print(df[df['Year']==2024][['Name','Author','User Rating']].to_string(index=False))

print("\n📊 2025 BOOKS:")
print(df[df['Year']==2025][['Name','Author','User Rating']].to_string(index=False))

# Chart 1
plt.figure(figsize=(6,4))
df['Genre'].value_counts().plot(kind='bar',color=['steelblue','coral'],edgecolor='black')
plt.title('Fiction vs Non Fiction')
plt.xticks(rotation=0)
plt.tight_layout()
plt.savefig('charts/1_genre.png')
plt.show()

# Chart 2
plt.figure(figsize=(8,5))
df['User Rating'].hist(bins=8,color='steelblue',edgecolor='black')
plt.title('Ratings Distribution')
plt.tight_layout()
plt.savefig('charts/2_ratings.png')
plt.show()

# Chart 3
plt.figure(figsize=(10,5))
df['Year'].value_counts().sort_index().plot(kind='bar',color='coral',edgecolor='black')
plt.title('Books Per Year')
plt.xticks(rotation=45)
plt.tight_layout()
plt.savefig('charts/3_per_year.png')
plt.show()

# Chart 4
plt.figure(figsize=(10,5))
top5 = df.nlargest(5,'Reviews')
plt.barh(top5['Name'], top5['Reviews'], color='steelblue')
plt.title('Top 5 Most Reviewed')
plt.tight_layout()
plt.savefig('charts/4_top5.png')
plt.show()

# Chart 5
plt.figure(figsize=(10,5))
df.groupby('Year')['User Rating'].mean().plot(marker='o',color='steelblue',linewidth=2)
plt.title('Average Rating by Year')
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig('charts/5_trend.png')
plt.show()

print("\n✅ DONE! Check the charts folder!")