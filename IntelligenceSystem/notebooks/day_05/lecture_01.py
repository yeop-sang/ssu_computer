# - 선형 모델 "분류"

# 분류형 데이터 셋
import pandas as pd
from sklearn.datasets import load_breast_cancer

data = load_breast_cancer()

# 정답변수
X = pd.DataFrame(data.data,
                 columns=data.feature_names)
# 종속변수 1차원 배열은 Series로 쓰자
y = pd.Series(data.target)

print(X.info())
X.isnull().sum()
print(X.describe(include='all'))

print(y.head())
print(y.value_counts())
print(y.value_counts() / len(y))

# 우리가 항상 봐야하는 건 scale 정보
# descision tree 만 논외

# test data 추출하기
from sklearn.model_selection import train_test_split

X_train, X_test, y_train, y_test = train_test_split(X, y,
                                                    test_size=0.3,
                                                    # 가장 중요한 개념, fitting 과 testing 상황을 동일하게 유지하기 위함
                                                    random_state=11,
                                                    # 추가적으로 중복 안되게 비율을 맞춤, 0과 1밖에 없기때문, 추나추출기법
                                                    stratify=y,
                                                    )

X_train.shape
X_test.shape
y_train.value_counts() / len(y_train)
y_test.value_counts() / len(y_test)

# 레그레이션 - 가중치가 널띰, 특정 특성에만 데이터가 많아짐
# 릿지 - 선형 모델 가중치랑 절편, 모든 특성에 대해서 가중치를 비슷하게 줌(0 주변에 박혀있음),
#       제약(알파)이 커지면 커질 수록 가중치를 더 0에 수렴시킴, 작아지면 레그레이션과 거의 동일
# 라소 - 예측보단 분석할때, 특성을 선택하는 제약을 걸 수 있음, 중요도가 높은 특성을 추출할때 테스트할 수 있음
from sklearn.linear_model import LogisticRegression

# 회귀 분석의 목적, 가중치를 적정히 할당해줘서 그 값을 추측할 수 있도록함
# 분류의 목적, 특정 값들을 적절하게 분류할 수 있는 선을 찾는 것

# L1이랑 L2 이용해서 제약조건을 줄 수 있음.
# C 제약을 풀어주는 조건, 제약을 작게하면 할 수록 score가 떨어짐, 커지면 커질 수록 과적합 됨
# class_weight 모델은 데이터가 많은 쪽으로 피팅이 된다, 따라서 특정 특성에 가중치를 주고 싶을때 사용함
# max_iter은 반복할 횟수
model = LogisticRegression(penalty="l2", C=1,
                           # class_weight='balanced',
                           class_weight=None,
                           max_iter=10000,
                           random_state=1,
                           n_jobs=6,
                           solver='lbfgs',
                           verbose=0
                           )

model.fit(X_train, y_train)

model.score(X_train, y_train)

model.score(X_test, y_test)

# 가중치 값 확인(분석할때는 가중치 값(==중요한 정도) 제대로 확인)
print(f'coef_ : {model.coef_}')
print(f'len(coef_) : {len(model.coef_[0])}')

# 절편 값 학인
print(f'intercept_ : {model.intercept_}')

"""
본류 모델의 평가 방법
1. 정확도
    - 전체 데이터에서 정답의 비율
    - 실제로 원하는 정답을 예측하지 못하는 오류가 있음
2. **정밀도(중요한 시험 범위)
    - 모델이 예측한 결과에서 정답인 비율
      (각 클래스 별로 분류된 값이 반환, 예) 암인 환자라고 판단한 것의 정확도와 아닌 환자라고 판단한 것의 정확도)
    - 모델이 예측한 값의 정확도
    - 정밀도가 굉장히 높은 모델은 재현율이 떨어짐
3. **재현율(중요한 시험 범위)
    - 실제 데이터 중 모델이 정답으로 예측한 비율
      (각 클래스 별로 분류된 값이 반환)

재현율과 정밀도의 적절한 비율을 맞춰주는 것이 중요함
"""

# 전체 데이터에서 얼마나 맞췄는지 표현해 주는 것
from sklearn.metrics import accuracy_score
# 정밀도를 확인하기 위한 클레스의 값이 필요
from sklearn.metrics import precision_score
# 재현율을 확인하기 위한 클래스의 값이 필요
from sklearn.metrics import recall_score

# 혼동행렬
from sklearn.metrics import confusion_matrix

pred = model.predict(X_train)
cm = confusion_matrix(y_train, y_pred=pred)

cm
"""
[    모델이 예측한    0         1
실제 정답   0       [139,       9],
실제 정답   1       [6,         244]
]
"""
# 정밀도
# 0에 대한 정밀도 : cm[0][0] / (cm[0][0] + cm[1][0])
ps = precision_score(y_train, pred)

ps
# 재현율 
# 0에 대한 재현율 : cm[0][0] / (cm[0][0] + cm[0][1])
rs = recall_score(y_train, pred, pos_label=0)

rs

# 정밀도가 낮고 재현율이 높은 경우, 해당 값에 대한 양이 데이터 내에 많다.

# 1에 대한 정밀도 : cm[1][1] / (cm[0][1] + cm[1][1])
ps1 = precision_score(y_train, pred, pos_label=1)
ps1

# 1에 대한 재현율 : cm[1][1] / (cm[1][0] + cm[1][1])
rs1 = recall_score(y_train, pred, pos_label=1)
rs1






















