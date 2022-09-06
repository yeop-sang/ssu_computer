my_random_seed = 80
target_column = 'SalePrice'

import pandas as pd
from sklearn.preprocessing import LabelEncoder, RobustScaler

pd.options.display.max_columns = 100
fname = './data/house_prices.csv'
data = pd.read_csv(fname, header='infer', sep=',')

y = data.SalePrice

# 비어있는 데이터 제거
removed_columns = [cname for cname in data.columns if data[cname].isnull().sum() != 0]
data.drop(columns=removed_columns, inplace=True)

# 문자열 데이터 추출
X_obj = [cname for cname in data.columns if data[cname].dtype not in ['int64', 'float64']]
X_obj = data[X_obj]
# 라벨 인코딩
for col in X_obj.columns:
    X_obj[col] = LabelEncoder().fit_transform(X_obj[col])
y = LabelEncoder().fit_transform(y)

# 숫자 데이터 추출
X_num = [cname for cname in data.columns if data[cname].dtype in ['int64', 'float64']]
X_num = data[X_num]

# RobustScaler
X_num = RobustScaler().fit_transform(X_num)

# 합침
X = pd.concat([pd.DataFrame(X_num), pd.DataFrame(X_obj)], axis=1)

# test 분리
from sklearn.model_selection import train_test_split

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, random_state=my_random_seed)

from sklearn.linear_model import LinearRegression
from sklearn.svm import SVR
from sklearn.tree import DecisionTreeRegressor

from sklearn.metrics import mean_absolute_error

lmodel = LinearRegression(
    n_jobs=4,
)

lmodel.fit(X_train, y_train)

smodel = SVR(
)

smodel.fit(X_train, y_train)

dmodel = DecisionTreeRegressor(
    random_state=my_random_seed
)

dmodel.fit(X_train, y_train)

print(mean_absolute_error(y, lmodel.predict(X)))
