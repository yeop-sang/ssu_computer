my_random_seed = 80
target_column = 'Transported'

import pandas as pd
from sklearn.preprocessing import OneHotEncoder, MinMaxScaler, LabelEncoder

pd.options.display.max_columns=100
fname = './data/space_shiip_titanic.csv'
data = pd.read_csv(fname, header='infer', sep=',')

y = data.Transported

# 비어있는 데이터 제거
removed_columns = [cname for cname in data.columns if data[cname].isnull().sum() != 0]
data.dropna(subset=removed_columns)

# 문자열 데이터 추출
X_obj = [cname for cname in data.columns if data[cname].dtype not in ['int64', 'float64']]
X_obj = [X_obj]
# 인코딩
encoder = OneHotEncoder(sparse=False)
X_obj = encoder.fit_transform(X_obj)

y = LabelEncoder().fit_transform(y)

# 숫자 데이터 추출
X_num = [cname for cname in data.columns if data[cname].dtype in ['int64', 'float64']]
X_num = data[X_num]

# RobustScaler
X_num = MinMaxScaler().fit_transform(X_num)
X_num=pd.DataFrame(X_num).reset_index(inplace=True)
X_obj=pd.DataFrame(X_obj).reset_index(inplace=True)
# 합침
X = pd.concat([X_num, X_obj], axis=1)

# test 분리
from sklearn.model_selection import train_test_split
X_train, X_test, y_train, y_test = train_test_split(X, y, stratify=y ,random_state=my_random_seed)

from sklearn.linear_model import LogisticRegression
from sklearn.svm import SVC
from sklearn.tree import DecisionTreeClassifier

from sklearn.metrics import mean_absolute_error

lmodel = LogisticRegression(
    n_jobs=4,
    random_state=my_random_seed
)

lmodel.fit(X_train, y_train)

smodel = SVC(
    random_state=my_random_seed
)

smodel.fit(X_train, y_train)

dmodel = DecisionTreeClassifier(
    random_state=my_random_seed
)

dmodel.fit(X_train, y_train)

print(mean_absolute_error(y, lmodel.predict(X)))

