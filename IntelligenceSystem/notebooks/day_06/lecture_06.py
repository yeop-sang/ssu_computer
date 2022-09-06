# -*- coding: utf-8 -*-

import pandas as pd
pd.options.display.max_columns=100

from sklearn.datasets import load_diabetes

data = load_diabetes()

X = pd.DataFrame(data.data, columns=data.feature_names)
y = pd.Series(data.target)

X.head()
X.info()
X.isnull().sum()

X.describe()

y.head()
y.value_counts()

from sklearn.model_selection import train_test_split
X_train,X_test,y_train,y_test=train_test_split(X,y,
                                               test_size=0.3,                                               
                                               random_state=1)

# 당뇨 수치 데이터를 사용하여 앙상블 기반의 회귀분석 모델을 구축하세요.
# 모델을 구축한 후 학습, 테스트 데이터에 대한 평균 절대 오차를 출력하여
# 모델의 적합성을 평가하세요.
# - 앙상블 모델은 배깅, 그레디언트 부스팅을 사용하세요.
# - 수업 종료 후,  LMS 시스템 과제에 코드와 모델의 적합성 평가 결과
#   를 이미지로 첨부해주세요.
from sklearn.ensemble import GradientBoostingClassifier

model = GradientBoostingClassifier(n_estimators=100,
                                   learning_rate=0.1,
                                   subsample=0.2,
                                   max_depth=1,
                                   random_state=1)

model.fit(X_train, y_train)

# 평가
score = model.score(X_train, y_train)
print(f'Score (train) : {score} ')

score = model.score(X_test, y_test)
print(f'Score (test) : {score} ')










