/*
    Copyright (c) 2017 TOSHIBA Digital Solutions Corporation.

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "Query.h"

namespace griddb {

    Query::Query(GSQuery *query, GSContainerInfo *containerInfo, GSRow *gsRow) : mQuery(query),
            mContainerInfo(containerInfo), mRow(gsRow) {
    }
    Query::~Query() {
        close();
    }
    /**
     * Release all resources created by this Query object.
     */
    void Query::close() {
        if (mQuery) {
            gsCloseQuery(&mQuery);
            mQuery = NULL;
        }
    }
    /**
     * Fetch data from query result.
     */
    RowSet* Query::fetch(bool for_update) {
        GSRowSet *rowSet;
        // Call method from C-Api.
        GSBool gsForUpdate = (for_update == true ? GS_TRUE:GS_FALSE);
        GSResult ret = gsFetch(mQuery, gsForUpdate, &rowSet);

        // Check ret, if error, throw exception
        if (ret != GS_RESULT_OK) {
            throw GSException(mQuery, ret);
        }

        return new RowSet(rowSet, mContainerInfo, mRow);
    }
    /**
     * Get row set. Convert from C-Api: gsGetRowSet
     */
    RowSet* Query::get_row_set() {
        GSRowSet *rowSet;
        GSResult ret = gsGetRowSet(mQuery, &rowSet);

        // Check ret, if error, throw exception
        if (ret != GS_RESULT_OK) {
            throw GSException(mQuery, ret);
        }

        return new RowSet(rowSet, mContainerInfo, mRow);
    }
    /**
     * Sets an fetch option of integer type for a result acquisition.
     */
    void Query::set_fetch_option_integer(GSFetchOption fetchOption, int32_t value) {
        GSResult ret = gsSetFetchOption(mQuery, fetchOption, &value, GS_TYPE_INTEGER);
        if (ret != GS_RESULT_OK) {
            throw GSException(mQuery, ret);
        }
    }
    /**
     * Sets an fetch option of long type for a result acquisition.
     */
    void Query::set_fetch_option_long(GSFetchOption fetchOption, int64_t value) {
        GSResult ret = gsSetFetchOption(mQuery, fetchOption, &value, GS_TYPE_LONG);
        if (ret != GS_RESULT_OK) {
            throw GSException(mQuery, ret);
        }
    }
    /**
     * Get raw pointer of GSQuery
     */
    GSQuery* Query::gs_ptr() {
        return mQuery;
    }
    /**
     * Set fetch limit option
     */
    void Query::set_fetch_options(int limit, bool partial){
    	if (limit >= 0) {
	        GSFetchOption fetchOption = GS_FETCH_LIMIT;
	        GSResult ret = gsSetFetchOption(mQuery, fetchOption, &limit, GS_TYPE_INTEGER);
	        if (ret != GS_RESULT_OK) {
	            throw GSException(mQuery, ret);
	        }
    	}
        if (partial == true) {
#if GS_COMPATIBILITY_SUPPORT_4_0
            GSFetchOption fetchOption = GS_FETCH_PARTIAL_EXECUTION;
            GSResult ret = gsSetFetchOption(mQuery, fetchOption, &partial, GS_TYPE_BOOL);
            if (ret != GS_RESULT_OK) {
                throw GSException(mQuery, ret);
            }
#endif
        }
    }
}
