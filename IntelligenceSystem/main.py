my_random_seed = 80
target_column = 'SalePrice'

import pandas as pd
from sklearn.preprocessing import LabelEncoder, RobustScaler

pd.options.display.max_columns=100
fname = './data/house_prices.csv'
data = pd.read_csv(fname, header='infer', sep=',')

y = data[target_column]

# 비어있는 데이터 제거
removed_columns = [cname for cname in data.columns if data[cname].isnull().sum() != 0]
# 유니크 한 값들 제거
# removed_columns+=[cname for cname in data.columns if len(data[cname].unique()) > ]

data.drop(columns=removed_columns, inplace=True)

# 문자열 데이터 추출
X_obj = [cname for cname in data.columns if data[cname].dtype not in ['int64', 'float64']]
# 라벨 인코딩
X_obj = LabelEncoder().fit_transform(X_obj)

# 숫자 데이터 추출
X_num = [cname for cname in data.columns if data[cname].dtype in ['int64', 'float64']]

# RobustScaler
X_num = RobustScaler().fit_transform(X_num)

# 합침
X = pd.concat([X_num, X_obj], axis=1)

# test 분리
from sklearn.model_selection import train_test_split
X_train, X_test, y_train, y_test =

from sklearn.linear_model import LinearRegression
from sklearn.svm import SVR
from sklearn.tree import DecisionTreeRegressor

from sklearn.metrics import mean_absolute_error
print()
