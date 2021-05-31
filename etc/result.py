import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd

sns.set()

# jede instanzgröße und zeit ein balken

j30 = pd.read_csv("phase_1_j30.csv")
j60 = pd.read_csv("phase_1_j60.csv")
j120 = pd.read_csv("phase_1_j120.csv")

j30_avgseed = j30.groupby(["instance_nr", "time"])["makespan"].mean().reset_index("instance_nr").reset_index("time")
j60_avgseed = j60.groupby(["instance_nr", "time"])["makespan"].mean().reset_index("instance_nr").reset_index("time")
j120_avgseed = j120.groupby(["instance_nr", "time"])["makespan"].mean().reset_index("instance_nr").reset_index("time")

j30_time = j30_avgseed.groupby("time").mean().reset_index("time")
j60_time = j60_avgseed.groupby("time").mean().reset_index("time")
j120_time = j120_avgseed.groupby("time").mean().reset_index("time")

j30_time["instance"] = "j30"
j60_time["instance"] = "j60"
j120_time["instance"] = "j120"

j_all = pd.concat([j30_avgseed, j60_avgseed, j120_avgseed])

j_time = pd.concat([j30_time, j60_time, j120_time])

print(j_time)

#sns.barplot(x=j30_avgseed.instance_nr, y=j30_avgseed.makespan, hue=j30_avgseed.time)
sns.barplot(x=j_time.instance,  y = j_time.makespan, hue=j_time.time)
plt.show()

