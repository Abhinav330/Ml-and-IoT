import joblib

model = joblib.load('temp.pkl')

pp = int(input("Enter °C value at which you want to bath : \t"))
print("\n")
vall = []
for i in range(23):
    val = int(model.predict([[i]]))
    # print(val)
    if val == pp:
        vall.append(i)

if len(vall) == 0:
    print("Value Not Found")
else :
    print(f'You can Bath between :')
    for i in vall:
        if i == vall[-1]:
            print(f"{i}:00 - {i+1}:00")
        else :
            print(f"{i}:00 - {i+1}:00  or ")

print("\n")