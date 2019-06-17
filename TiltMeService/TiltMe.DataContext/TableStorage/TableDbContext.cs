using System;
using System.Collections.Generic;
using System.Configuration;
using System.Threading.Tasks;
using AutoMapper.Configuration;
using Microsoft.WindowsAzure.Storage;
using Microsoft.WindowsAzure.Storage.Table;
using TiltMe.Models;

namespace TiltMe.DataContext.TableStorage
{
    public class TableDbContext
    {
        protected CloudTableClient _tableClient;

        public TableDbContext(IApplicationConfiguration applicationConfiguration)
        {
            CloudStorageAccount storageAccount = CloudStorageAccount.Parse(applicationConfiguration.TableStorageConnectionString);
            _tableClient = storageAccount.CreateCloudTableClient();
        }

        protected async Task<bool> Exists<T>(CloudTable table, T entity) where T : TableEntity
        {
            try
            {
                TableOperation retrieveOperation = TableOperation.Retrieve(entity.PartitionKey, entity.RowKey);
                TableResult retrievedResult = await table.ExecuteAsync(retrieveOperation);
                //Debug.WriteLine("Exists " + (retrievedResult.Result != null));
                return retrievedResult.Result != null;
            }
            catch (Exception e)
            {
                System.Diagnostics.Trace.TraceError("Error checking entity", e);
                return false;
            }
        }

        protected async Task<bool> Exists(CloudTable table, string partitionKey, string rowkey)
        {
            try
            {
                TableOperation retrieveOperation = TableOperation.Retrieve(partitionKey, rowkey);
                TableResult retrievedResult = await table.ExecuteAsync(retrieveOperation);
                //Debug.WriteLine("Exists " + (retrievedResult.Result != null));
                return retrievedResult.Result != null;
            }
            catch (Exception e)
            {
                System.Diagnostics.Trace.TraceError("Error checking entity", e);
                return false;
            }
        }

        protected async Task<T> Get<T>(CloudTable table, string partitionKey, string rowKey) where T : ITableEntity
        {
            try
            {
                TableOperation get = TableOperation.Retrieve<T>(partitionKey, rowKey);
                TableResult result = await table.ExecuteAsync(get);
                if (result.Result != null)
                {
                    return (T)result.Result;
                }
            }
            catch (Exception e)
            {
                System.Diagnostics.Trace.TraceError("Error getting entity", e);                
            }
            return default(T);
        }

        protected async Task<IList<T>> GetAll<T>(CloudTable table, string partitionKey) where T : ITableEntity, new()
        {
            try
            {
                String partitionFilter = TableQuery.GenerateFilterCondition(
                    "PartitionKey",
                    QueryComparisons.Equal,
                    partitionKey);

                var tableQuery = new TableQuery<T>().Where(partitionFilter);
                TableContinuationToken token = null;
                List<T> items = new List<T>();

                do
                {
                    TableQuerySegment<T> seg = await table.ExecuteQuerySegmentedAsync<T>(tableQuery, token);
                    token = seg.ContinuationToken;
                    items.AddRange(seg);

                } while (token != null);

                return items;
            }
            catch (Exception e)
            {
                System.Diagnostics.Trace.TraceError("Error getting entity", e);
            }

            return null;
        }

        protected async Task<T> Get<T>(CloudTable table, T entity) where T : ITableEntity
        {
            try
            {
                TableOperation get = TableOperation.Retrieve<T>(entity.PartitionKey, entity.RowKey);
                TableResult result = await table.ExecuteAsync(get);
                if (result.Result != null)
                {
                    return (T)result.Result;
                }
            }
            catch (Exception e)
            {
                System.Diagnostics.Trace.TraceError("Error getting entity", e);
            }
            return default(T);
        }

        protected async Task<bool> Insert<T>(CloudTable table, T entity) where T : ITableEntity
        {
            try
            {
                TableOperation insert = TableOperation.Insert(entity);
                TableResult result = await table.ExecuteAsync(insert);
                return result.Result != null;
            }
            catch (Exception e)
            {
                System.Diagnostics.Trace.TraceError("Error checking entity", e);
                return false;
            }
        }

        protected T Query<T>(Func<T> query, string desc)
        {
            lock (_tableClient)
            {
                try
                {
                    return query();
                }
                catch (Exception e)
                {
                    System.Diagnostics.Trace.TraceError("Query failed. " + desc, e);
                    //Trace.TraceInformation("Query failed. " + desc, e);
                }
                return default(T);
            }
        }

        protected void Query(Action query, string desc)
        {
            lock (_tableClient)
            {
                try
                {
                    query();
                }
                catch (Exception e)
                {
                    System.Diagnostics.Trace.TraceError("Query failed. " + desc, e);
                    //Trace.TraceInformation("Query failed. " + desc, e);
                }
            }
        }

    }
}
