# -*- coding: utf-8 -*-

# 서포트 벡터 머신 (분류)

import pandas as pd
pd.options.display.max_columns=100
from sklearn.datasets import load_breast_cancer

data = load_breast_cancer()

X = pd.DataFrame(data.data, 
                 columns=data.feature_names)
y = pd.Series(data.target)

X.info()
X.isnull().sum()
X.describe(include='all')

y.head()
y.value_counts()
y.value_counts() / len(y)

from sklearn.model_selection import train_test_split

splits = train_test_split(X, y,
                          test_size=0.3,
                          random_state=10,
                          stratify=y)

X_train=splits[0]
X_test=splits[1]
y_train=splits[2]
y_test=splits[-1]

X_train.head()
X_test.head()

X_train.shape
X_test.shape

y_train.value_counts() / len(y_train)
y_test.value_counts() / len(y_test)


from sklearn.svm import SVC, LinearSVC

model = SVC(C=1.0,
            gamma='scale',
            class_weight='balanced',
            random_state=1)
model.fit(X_train, y_train)

model.score(X_train, y_train)
model.score(X_test, y_test)

# 서포트 벡터의 가중치 값 확인
# - 커널 방법을 선형으로 (linear)로 설정한 경우 사용
print(f'coef_ : {model.coef_}')
print(f'intercept_ : {model.intercept_}')


proba = model.predict_proba(X_train[:5])
proba

pred = model.predict(X_train[:5])
pred

df = model.decision_function(X_train[-10:])
df

pred = model.predict(X_train[-10:])
pred

y_train[:5]

# 분류 모델의 평가 방법
# 1. 정확도
#  - 전체 데이터에서 정답으로 맞춘 비율 
#  - 머신러닝 모델의 score 메소드
#  - 분류하고자 하는 각각의 클래스의 비율이
#    동일한 경우에만 사용

# 2. 정밀도
#  - 집합 : 머신러닝 모델이 예측한 결과
#  - 위의 집합에서 각각의 클래스 별 정답 비율

# 3. 재현율
#  - 집합 : 학습 데이터 셋
#  - 위의 집합에서 머신러닝 모델이 예측한 정답 비율

# 혼동행렬
from sklearn.metrics import confusion_matrix

pred = model.predict(X_train)
cm = confusion_matrix(y_train, pred)
cm

y_train.value_counts()

# array([[148,   0],
#        [ 42, 208]], dtype=int64)

# array([[142,   6],
#        [  8, 242]], dtype=int64)

# 머신러닝 모델의 예측    0    1  
# 실제 0인 데이터     [[141,   7]
# 실제 1인 데이터      [  5, 245]]

# 정밀도(0) : 141 / (141 + 5)
# 재현율(0) : 141 / (141 + 7)

# 정확도
from sklearn.metrics import accuracy_score
# 정밀도
from sklearn.metrics import precision_score
# 재현율
from sklearn.metrics import recall_score

pred = model.predict(X_train)
ps = precision_score(y_train, pred, 
                     pos_label=0)
ps

rs = recall_score(y_train, pred, 
                     pos_label=0)
rs


















