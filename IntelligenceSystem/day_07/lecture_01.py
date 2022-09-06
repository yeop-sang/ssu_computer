# -*- coding: utf-8 -*-

import pandas as pd
pd.options.display.max_columns=100
pd.options.display.max_rows=10

fname_input = './day_07/titanic.csv'
data = pd.read_csv(fname_input,
                   header='infer',
                   sep=',')

# 데이터에 문자열이 보이는 것을 확인
print(data.head())

# float64(2), int64(5), object(5)
print(data.info())

print(data.describe())

# 수치형 데이터에서도 ID 성격의 데이터가
# 존재할 수 있음
# - 데이터의 개수가 모두 1
# - 하단의 Length 정보가 전체 데이터의 개수와
#   동일함
print(data.PassengerId.value_counts())

# 문자열에 대해서 기초 통계정보를 확인
# - 결측이 아닌 데이터의 개수
# - 각 컬럼의 중복을 제거시킨 데이터의 개수
# - 머신러닝에 불필요한 정보를 확인한 후
#   제거할 수 있어야함
#   ['PassengerId', 'Name', 'Ticket', 'Cabin']
print(data.describe(include=object))

# 불필요한 컬럼의 제거 코드
# - DataFrame의 drop 메소드
# - drop(columns=[제거할 컬럼명 ..])
# - inplace 매개변수 : 실행의 결과를
#   현재 변수(data)에 반영할 지 여부를 지정
data2 = data.drop(
    columns=['PassengerId','Name','Ticket','Cabin'],
    inplace=False)

# 결측 데이터를 확인
print(data2.info())
print(data2.isnull().sum())

# 결측 데이터의 처리 방법
# 1. 결측 데이터가 포함된 레코드(열)을 제거
# (Age와 Embarked 의 데이터 개수가 많으므로
# 제거는 어려움)
# 2. 결측 데이터가 포함된 레코드(행)을 제거
# (Embarked 컬럼의 경우 결측이 2개뿐이므로
# 행 자체를 제거할 수 있음 - 고려의 대상)

# 3. 기초 통계로 결측치를 대체함
# (평균, 중심값, 최빈값 - 수치형,
#  최빈값 - 문자열)

# 4. 지도학습 기반의 머신러닝 모델을 구축하여
# 예측한 값으로 결측데이터를 대치함
# (Age 컬럼 - 결측치가 많지만 결측이 아닌 데이터의
# 개수가 월등히 많으므로 학습 고려 대상)

# 5. 준지도학습, 비지도학습 기반의 머신러닝
# 모델을 구축하여 모델의 예측값으로 결측치를
# 대체함 - 클러스터링
# (Cabin 컬럼의 경우)

data3 = data2.dropna(
    subset=['Age','Embarked'])

print(data3.info())
print(data3.isnull().sum())

# 문자열 데이터 전처리의 필요성
# - 머신러닝 알고리즘이 문자열 처리 X
# - 문자열을 수치형 데이터로 변환

# 문자열 데이터이 전처리 위치
# - 기본적으로 데이터의 분할전에 하는 것이 원칙

# 아래의 성별 데이터는 학습에도 남성/여성
# 테스트에도 남성/여성으로 포함딜 수 있을까?
print(data.Sex.value_counts())
# 아래의 성별 데이터는 학습에도 S/C/Q
# 테스트에도 S/C/Q으로 포함딜 수 있을까?
print(data.Embarked.value_counts())

# 문자열 데이터의 전처리는 기본적으로
# 매칭 방법을 사용
# - 전처리 대상에 존재하는 문자열 데이터를
# 수치데이터로 연계하여 변환하는 방식을 사용


# 문자열 데이터의 전처리 방식
# 1. 라벨인코딩
# - 특정 문자열 데이터를 정수와 매칭하여
#   단순 변환하는 방식
# - 학습 데이터에 남성/여성이 존재하는 경우
#  EX) 남성 / 여성 -> 0 / 1
#  EX) S / Q / C -> 0 / 1 / 2

# 라벨 인코딩은 일반적으로 정답 데이터(y)가 
# 문자열로 구성되어 있는 경우 사용함
# (필수는 아님)

# 반면 라벨 인코딩은 설명변수 X에는 잘
# 사용하지 않음...
# EX) Sex * W
#     Embarked * W

# 2. 원핫인코딩
# - 유일한 문자열의 개수만큼 컬럼을 생성하여
#   그 중 하나의 위치에만 1을 대입하는 방식
# - 학습 데이터에 남성/여성이 존재하는 경우
#  EX) 남성 / 여성 -> 1 0 / 0 1
#  EX) S / Q / C -> 1 0 0 / 0 1 0 / 0 0 1

# - 일반적으로 설명변수(X)에 포함된 문자열
# 데이터으 전처리에 활용됨

# - 메모리의 낭비가 심하므로 유일한 값의 
# 케이스를 줄일 수 있는 방향을 접근하는 것이
# 유의미하다.

# 문자열 전처리는 수치형 자료의 전처리와는
# 과정이 독립적이므로 데이터를 분할하는 것이
# 작성에 편리함

X = data3.iloc[:, 1:]
y = data3.Survived

# 수치형 데이터의 컬럼명
X_num = [cname for cname in X.columns 
         if X[cname].dtype in ['int64','float64'] ]
# 문자형 데이터의 컬럼명
X_obj = [cname for cname in X.columns 
         if X[cname].dtype not in ['int64','float64'] ]

print(X['Pclass'].dtype)
print(X['Age'].dtype)
print(X['Sex'].dtype)


X_num = X[X_num]
X_obj = X[X_obj]

print(X_num.info())
print(X_obj.info())

from sklearn.preprocessing import OneHotEncoder
# 원핫인코더의 주요 파라메터
# - sparse : 희소행렬 생성 여부
# EX) S / Q / C --> 1 0 0 / 0 1 0 / 0 0 1
#       희소행렬 --> (0) 1 / (1) 1 / (2) 1
# - handle_unknown : 학습 과정에서 인지하지 못한
# 문자열 값에 대한 처리 프로세스 정의
# ('error', 'ignore')
encoder = OneHotEncoder(sparse=True,
                        handle_unknown='ignore')

# 사이킷 런의 전처리 클래스들은 학습용 클래스와 유사하게
# 아래와 같은 메소드를 제공함
# 1. fit 메소드 : 전처리 과정에 필요한 정보를 수집
# 2. transform 메소드 : fit 메소드에서 인지한 결과를
# 바탕으로 데이터를 변환하여 반환하는 메소드
# 3. fit_transform 메소드 : 1번과 2번의 과정을 한번에...

X_obj_encoded = encoder.fit_transform(X_obj)

#       Sex Embarked
# 0    male        S
# 1  female        C
# 2  female        S
# 3  female        S
# 4    male        S
print(X_obj.head())

# 0 1 0 0 1
# 1 0 1 0 0

# [[0. 1. 0. 0. 1.]
#  [1. 0. 1. 0. 0.]
#  [1. 0. 0. 0. 1.]
#  [1. 0. 0. 0. 1.]
#  [0. 1. 0. 0. 1.]]
print(X_obj_encoded[:5])


# 주의사항 
# - 모든 사이킷런의 전처리 클래스들은
#   transform 메소드의 결과가 numpy 배열로 반환
#   (pandas의 데이터프레임이 아님!!!)

print(encoder.categories_)
print(encoder.feature_names_in_)

X_obj_encoded = pd.DataFrame(X_obj_encoded,
                             columns=['s_f','s_m',
                                      'e_C','e_Q','e_S'])

print(X_obj_encoded)


# 전처리된 데이터를 결합하여
# 설명변수 X를 생성
print(X_num.info())
print(X_obj_encoded.info())

X_num.reset_index(inplace=True)
X_obj_encoded.reset_index(inplace=True)

# concat 메소드를 사용하여 데이터프레임을 결합
X = pd.concat([X_num, X_obj_encoded],
              axis = 1)

print(X.info())

# 종속변수 확인
# - 0 / 1에 대한 편향이 일부 존재
# - 학습 시 class 별 가중치를 제어할 필요성이 있음
print(y.value_counts())
print(y.value_counts() / len(y))

from sklearn.model_selection import train_test_split
X_train,X_test,y_train,y_test=train_test_split(X,y,
                                               test_size=0.3,
                                               stratify=y,
                                               random_state=0)

from sklearn.ensemble import RandomForestClassifier
from sklearn.ensemble import GradientBoostingClassifier

model = RandomForestClassifier(n_estimators=100,
                               max_depth=None,
                               max_samples=1.0,
                               class_weight='balanced',
                               n_jobs=-1,
                               random_state=0)

model.fit(X_train, y_train)

score = model.score(X_train, y_train)
print(f'Score(Train) : {score}')

score = model.score(X_test, y_test)
print(f'Score(Test) : {score}')



























